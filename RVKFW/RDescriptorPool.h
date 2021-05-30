
#ifndef RDescriptorPool_h
#define RDescriptorPool_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RPhysicalDevice;
class RVertexBuffer;
class RSwapChain;

class RDescriptorPool: public RObject {
public:
    RDescriptorPool(std::shared_ptr<RPhysicalDevice> physicalDevice,
                    std::shared_ptr<RLogicalDevice> logicalDevice,
                    std::shared_ptr<RSwapChain> swapChain);

    ~RDescriptorPool() {
        destroy();
    }

    void create(uint32_t size, const std::vector<uint32_t> &sizePerBuffer);
    void destroy() override;

    const char * tag() const override {
        return "RDescriptorPool";
    }

    VkDescriptorPoolSize &poolSize() {
        return mPoolSize;
    }

    VkDescriptorPoolCreateInfo &poolInfo() {
        return mPoolInfo;
    }

    VkDescriptorSetAllocateInfo &setAllocInfo() {
        return mSetAllocInfo;
    }

    VkDescriptorPool handle() const {
        return mDescriptorPool;
    }

    VkDescriptorSetLayout *descriptorSetLayout() {
        return &mDescriptorSetLayout;
    }

    VkDescriptorSetLayoutBinding &layoutBinding() {
        return mLayoutBinding;
    }

    VkDescriptorSetLayoutCreateInfo &layoutInfo() {
        return mLayoutInfo;
    }

    std::size_t buffersCount() {
        return mBuffers.size();
    }

    std::shared_ptr<RVertexBuffer> buffer(int loc) {
        return mBuffers[loc];
    }

    const std::vector<VkDescriptorSet> &descriptorSets() const {
        return mDescriptorSets;
    }

protected:

    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RPhysicalDevice> mPhysicalDevice;
    std::weak_ptr<RSwapChain> mSwapChain;

    VkDescriptorSetLayoutBinding mLayoutBinding{};
    VkDescriptorSetLayoutCreateInfo mLayoutInfo{};

    VkDescriptorPoolSize mPoolSize{};
    VkDescriptorPoolCreateInfo mPoolInfo{};
    VkDescriptorSetAllocateInfo mSetAllocInfo{};

    std::vector<std::shared_ptr<RVertexBuffer>> mBuffers;

    VkDescriptorSetLayout mDescriptorSetLayout{VK_NULL_HANDLE};
    VkDescriptorPool mDescriptorPool{ VK_NULL_HANDLE};

    std::vector<VkDescriptorSet> mDescriptorSets{};
};

}

#endif
