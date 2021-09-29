
#include "RImageBuffer.h"
#include "RLogger.h"
#include "RBufferObject.h"
#include "RLogicalDevice.h"
#include "RPhysicalDevice.h"

#include <stb_image.h>

namespace rvkfw {

RImageBuffer::RImageBuffer(std::weak_ptr<RPhysicalDevice> physicalDevice,
             std::weak_ptr<RLogicalDevice> logicalDevice) :
        mPhysicalDevice{physicalDevice},
        mLogicalDevice{logicalDevice} {

    mImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    mImageInfo.imageType = VK_IMAGE_TYPE_2D;
    mImageInfo.extent.depth = 1;
    mImageInfo.mipLevels = 1;
    mImageInfo.arrayLayers = 1;
    mImageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    mImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    mImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    mImageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT |
            VK_IMAGE_USAGE_SAMPLED_BIT;
    mImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    mImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    mImageInfo.flags = 0;

}

void RImageBuffer::create(const std::string &imageFile) {
    if (mCreated.exchange(true)) {
        return;
    }

    auto logicalDevice = mLogicalDevice.lock();
    ASSERT_NOT_NULL(logicalDevice);

    auto physicalDevice = mPhysicalDevice.lock();
    ASSERT_NOT_NULL(physicalDevice);

    stbi_uc* pixels = stbi_load(imageFile.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb_alpha);
    if (!pixels) {
        LOG_ERROR(tag(), "Failed to load texture image!");
        throw std::runtime_error("Failed to load texture image!");
    }

    mImageSize = mWidth * mHeight * 4;

    mImageInfo.extent.width = static_cast<uint32_t>(mWidth);
    mImageInfo.extent.height = static_cast<uint32_t>(mHeight);

    auto bufferCreator = RBufferObject::Creator(mPhysicalDevice, mLogicalDevice);
    bufferCreator.bufferInfo().usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferCreator.setMemoryProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    mBuffer = bufferCreator.create(pixels, mImageSize);
    stbi_image_free(pixels);

    if (vkCreateImage(logicalDevice->handle(), &mImageInfo, nullptr, &mImage)!= VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create image!");
        throw std::runtime_error("Failed to create image!");
    }

    VkMemoryRequirements memRequirements{};
    vkGetImageMemoryRequirements(logicalDevice->handle(), mImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = mBuffer->findMemoryType(physicalDevice->memProperties(),
                                                        memRequirements.memoryTypeBits,
                                                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(logicalDevice->handle(), &allocInfo, nullptr,
                         &mTextureImageMemory) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to allocate image memory!");
        throw std::runtime_error("Failed to allocate image memory!");
    }

    vkBindImageMemory(logicalDevice->handle(), mImage, mTextureImageMemory, 0);
}

void RImageBuffer::destroy() {
    if (!mCreated.exchange(false)) {
        return;
    }

    if (auto logicalDevice = mLogicalDevice.lock()) {
        if (mImage != VK_NULL_HANDLE) {
            vkDestroyImage(logicalDevice->handle(), mImage, nullptr);
            mImage = VK_NULL_HANDLE;
        }
        if (mTextureImageMemory != VK_NULL_HANDLE) {
            vkFreeMemory(logicalDevice->handle(), mTextureImageMemory, nullptr);
            mTextureImageMemory = VK_NULL_HANDLE;
        }
    }

    mBuffer = nullptr;
}

}
