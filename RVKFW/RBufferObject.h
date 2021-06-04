
#ifndef RBufferObject_h
#define RBufferObject_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RPhysicalDevice;

class RBufferObject: public RObject {
public:
    RBufferObject(std::weak_ptr<RPhysicalDevice> physicalDevice,
                  std::weak_ptr<RLogicalDevice> logicalDevice);

    ~RBufferObject() {
        destroy();
    }

    void create(const void *data, int size);
    void destroy() override;
    void setData(const void *data, int size);

    const char * tag() const override {
        return "RVertexBuffer";
    }

    VkBuffer handle() const {
        return mBuffer;
    }

    VkBufferCreateInfo &bufferInfo() {
        return mBufferInfo;
    }

    VkMemoryRequirements &memRequirements() {
        return mMemRequirements;
    }

    void setMemoryProperties(const VkMemoryPropertyFlags &value) {
        mMemoryProperties = value;
    }

    uint32_t findMemoryType(const VkPhysicalDeviceMemoryProperties &memProperties,
                            uint32_t typeFilter, VkMemoryPropertyFlags properties);

protected:
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RPhysicalDevice> mPhysicalDevice;

    VkBufferCreateInfo mBufferInfo{};
    VkMemoryRequirements mMemRequirements{};

    VkBuffer mBuffer{ VK_NULL_HANDLE };
    VkDeviceMemory mBufferMemory{ VK_NULL_HANDLE };

    VkMemoryPropertyFlags mMemoryProperties{VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

public:
    class Creator {
    public:
        Creator(std::weak_ptr<RPhysicalDevice> physicalDevice,
                std::weak_ptr<RLogicalDevice> logicalDevice) :
            mObject{std::make_shared<RBufferObject>(physicalDevice, logicalDevice)} {
                mObject->preCreate();
        }

        VkBufferCreateInfo &bufferInfo() {
            return mObject->bufferInfo();
        }

        VkMemoryRequirements &memRequirements() {
            return mObject->memRequirements();
        }

        Creator &setMemoryProperties(const VkMemoryPropertyFlags &value) {
            mObject->setMemoryProperties(value);
            return *this;
        }

        std::shared_ptr<RBufferObject> create(const void *data, int size) {
            mObject->create(data, size);
            return mObject;
        }

    private:
        std::shared_ptr<RBufferObject> mObject;
    };
    friend Creator;
};

}

#endif
