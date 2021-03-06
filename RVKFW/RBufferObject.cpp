
#include "RBufferObject.h"
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"
#include "RLogger.h"

namespace rvkfw {

RBufferObject::RBufferObject(std::weak_ptr<RPhysicalDevice> physicalDevice,
                             std::weak_ptr<RLogicalDevice> logicalDevice) :
    mPhysicalDevice{physicalDevice},
    mLogicalDevice{logicalDevice} {

    mBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    mBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    mBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}

uint32_t RBufferObject::findMemoryType(const VkPhysicalDeviceMemoryProperties &memProperties,
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

void RBufferObject::create(const void *data, int size) {
    if (mCreated.exchange(true)) {
        return;
    }

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
                                               mMemoryProperties);

    if (vkAllocateMemory(logicalDevice->handle(), &allocInfo, nullptr, &mBufferMemory) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to allocate vertex buffer memory!")
        throw std::runtime_error("Failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(logicalDevice->handle(), mBuffer, mBufferMemory, 0);

    if (data) {
        setData(data, size);
    }
}

void RBufferObject::setData(const void *data, int size) {
    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    if (data) {
        void* mappedArea;
        vkMapMemory(logicalDevice->handle(), mBufferMemory, 0, mBufferInfo.size, 0, &mappedArea);
        memcpy(mappedArea, data, (size_t) mBufferInfo.size);

        vkUnmapMemory(logicalDevice->handle(), mBufferMemory);
    }

}


void RBufferObject::destroy() {
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
