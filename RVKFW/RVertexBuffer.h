
#ifndef RVertexBuffer_h
#define RVertexBuffer_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RPhysicalDevice;

class RVertexBuffer: public RObject {
public:
    RVertexBuffer(std::shared_ptr<RPhysicalDevice> physicalDevice,
                  std::shared_ptr<RLogicalDevice> logicalDevice);

    ~RVertexBuffer() {
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
    
protected:
    uint32_t findMemoryType(const VkPhysicalDeviceMemoryProperties &memProperties,
                            uint32_t typeFilter, VkMemoryPropertyFlags properties);

    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RPhysicalDevice> mPhysicalDevice;

    VkBufferCreateInfo mBufferInfo{};
    VkMemoryRequirements mMemRequirements{};

    VkBuffer mBuffer{ VK_NULL_HANDLE };
    VkDeviceMemory mBufferMemory{ VK_NULL_HANDLE };

    VkMemoryPropertyFlags mMemoryProperties{VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
};

}

#endif
