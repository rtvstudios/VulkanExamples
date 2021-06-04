
#ifndef RImageBuffer_h
#define RImageBuffer_h

#include "RObject.h"

namespace rvkfw {

class RPhysicalDevice;
class RLogicalDevice;
class RBufferObject;

class RImageBuffer: public RObject {
public:
    RImageBuffer(std::weak_ptr<RPhysicalDevice> physicalDevice,
                 std::weak_ptr<RLogicalDevice> logicalDevice);
    ~RImageBuffer() {
        destroy();
    }

    void create(const std::string &imageFile);
    void destroy() override;

    const char * tag() const override {
        return "RImage";
    }

    VkImageCreateInfo &imageInfo() {
        return mImageInfo;
    }

    VkImage handle() {
        return mImage;
    }

    VkDeviceMemory deviceMemoryHandle() {
        return mTextureImageMemory;
    }

protected:
    std::weak_ptr<RPhysicalDevice> mPhysicalDevice;
    std::weak_ptr<RLogicalDevice> mLogicalDevice;

    std::shared_ptr<RBufferObject> mBuffer;

    int mWidth{};
    int mHeight{};
    int mChannels{};

    VkDeviceSize mImageSize{};

    VkImageCreateInfo mImageInfo{};

    VkImage mImage{VK_NULL_HANDLE};
    VkDeviceMemory mTextureImageMemory{VK_NULL_HANDLE};

public:
    class Creator {
    public:
        Creator(std::weak_ptr<RPhysicalDevice> physicalDevice,
                std::weak_ptr<RLogicalDevice> logicalDevice) :
            mObject{ std::make_shared<RImageBuffer>(physicalDevice, logicalDevice)} {
                mObject->preCreate();
        }

        VkImageCreateInfo &imageInfo() {
            return mObject->imageInfo();
        }

        std::shared_ptr<RImageBuffer> create(const std::string &imageFile) {
            mObject->create(imageFile);
            return mObject;
        }

    private:
        std::shared_ptr<RImageBuffer> mObject;
    };
};

}

#endif
