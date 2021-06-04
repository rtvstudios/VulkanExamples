
#include "RDescriptorPool.h"
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"
#include "RLogger.h"
#include "RBufferObject.h"

namespace rvkfw {

RDescriptorPool::RDescriptorPool(std::shared_ptr<RPhysicalDevice> physicalDevice,
                                 std::shared_ptr<RLogicalDevice> logicalDevice,
                                 std::shared_ptr<RSwapChain> swapChain) :
    mPhysicalDevice(physicalDevice), mLogicalDevice(logicalDevice), mSwapChain(swapChain) {
        
    mLayoutBinding.binding = 0;
    mLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    mLayoutBinding.descriptorCount = 1;

    mLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    mLayoutBinding.pImmutableSamplers = nullptr;

    mLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    mLayoutInfo.bindingCount = 1;
    mLayoutInfo.pBindings = &mLayoutBinding;

    mPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    mPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    mPoolInfo.poolSizeCount = 1;
    mPoolInfo.pPoolSizes = &mPoolSize;

    mSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
}

void RDescriptorPool::create(uint32_t size, const std::vector<uint32_t> &sizePerBuffer) {
    if (mCreated.exchange(true)) {
        return;
    }

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);
    auto physicalDevice = mPhysicalDevice.lock();
    ASSERT_NOT_NULL(physicalDevice);

    mPoolSize.descriptorCount = size;

    mPoolInfo.maxSets = size;

    if (vkCreateDescriptorSetLayout(logicalDevice->handle(), &mLayoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create descriptor set layout!");
        throw std::runtime_error("Failed to create descriptor set layout!");
    }

    if (vkCreateDescriptorPool(logicalDevice->handle(), &mPoolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create descriptor pool!");
        throw std::runtime_error("Failed to create descriptor pool!");
    }

    for (int i=0; i<size; ++i) {
        auto bufferCreator = RBufferObject::Creator(physicalDevice, logicalDevice);

        bufferCreator.bufferInfo().usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferCreator.setMemoryProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        mBuffers.push_back(bufferCreator.create(nullptr, sizePerBuffer[i]));
    }

    std::vector<VkDescriptorSetLayout> layouts(size, mDescriptorSetLayout);

    mSetAllocInfo.descriptorPool = mDescriptorPool;
    mSetAllocInfo.descriptorSetCount = size;
    mSetAllocInfo.pSetLayouts = layouts.data();

    mDescriptorSets.resize(size);
    if (vkAllocateDescriptorSets(logicalDevice->handle(), &mSetAllocInfo, mDescriptorSets.data()) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to allocate descriptor sets!");
        throw std::runtime_error("Failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < size; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = mBuffers[i]->handle();
        bufferInfo.offset = 0;
        bufferInfo.range = sizePerBuffer[i];

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = mDescriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;

        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;

        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(logicalDevice->handle(), 1, &descriptorWrite, 0, nullptr);
    }
}

void RDescriptorPool::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }

    if ( auto logicalDevice = mLogicalDevice.lock()) {
        if (mDescriptorSetLayout != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(logicalDevice->handle(), mDescriptorSetLayout, nullptr);
            mDescriptorSetLayout = VK_NULL_HANDLE;
        }

        if (mDescriptorPool != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(logicalDevice->handle(), mDescriptorPool, nullptr);
            mDescriptorPool = VK_NULL_HANDLE;
        }
    }
}


}
