
#include "RVertexBuffer.h"
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"
#include "RLogger.h"

namespace rvkfw {

RVertexBuffer::RVertexBuffer(std::shared_ptr<RPhysicalDevice> physicalDevice,
                             std::shared_ptr<RLogicalDevice> logicalDevice) :
    mPhysicalDevice{physicalDevice},
    mLogicalDevice{logicalDevice} {

    mBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    mBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    mBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}

uint32_t RVertexBuffer::findMemoryType(const VkPhysicalDeviceMemoryProperties &memProperties,
                                       uint32_t typeFilter, VkMemoryPropertyFlags properties) {

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    LOG_ERROR(tag(), "Failed to find suitable memory type");
    throw std::runtime_error("Failed to find suitable memory type!");
}

void RVertexBuffer::create(const void *data, int size) {
    if (mCreated.exchange(true)) {
        return;
    }

    ASSERT_NOT_NULL(data);

    mBufferInfo.size = size;

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    auto physicalDevice = mPhysicalDevice.lock();
    ASSERT_NOT_NULL(physicalDevice);

    if (vkCreateBuffer(logicalDevice->handle(), &mBufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create vertex buffer");
        throw std::runtime_error("Failed to create vertex buffer!");
    }

    vkGetBufferMemoryRequirements(logicalDevice->handle(), mBuffer, &mMemRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    allocInfo.allocationSize = mMemRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice->memProperties(),
                                               mMemRequirements.memoryTypeBits,
                                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    
    if (vkAllocateMemory(logicalDevice->handle(), &allocInfo, nullptr, &mBufferMemory) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to allocate vertex buffer memory!")
        throw std::runtime_error("Failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(logicalDevice->handle(), mBuffer, mBufferMemory, 0);

    void* mappedArea;
    vkMapMemory(logicalDevice->handle(), mBufferMemory, 0, mBufferInfo.size, 0, &mappedArea);
    memcpy(mappedArea, data, (size_t) mBufferInfo.size);

    vkUnmapMemory(logicalDevice->handle(), mBufferMemory);
}

void RVertexBuffer::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }

    if (auto logicalDevice = mLogicalDevice.lock()) {
        vkDestroyBuffer(logicalDevice->handle(), mBuffer, nullptr);
        vkFreeMemory(logicalDevice->handle(), mBufferMemory, nullptr);
    } else {
        LOG_ERROR(tag(), "Logical device is null, could not destroy vertex buffer");
    }

}

}
