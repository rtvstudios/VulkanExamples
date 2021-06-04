
#ifndef RCommandPool_h
#define RCommandPool_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RShaderModule;
class RSwapChain;
class RRenderPass;

class RCommandPool: public RObject {
public:
    RCommandPool(std::shared_ptr<RLogicalDevice> logicalDevice);
    ~RCommandPool();

    void create();
    void destroy() override;

    const char * tag() const override {
        return "RCommandPool";
    }

    VkCommandPool handle() const {
        return mCommandPool;
    }

    VkCommandPoolCreateInfo &poolCreateInfo() {
        return mCreatePoolInfo;
    }

protected:
    std::weak_ptr<RLogicalDevice> mLogicalDevice;

    VkCommandPoolCreateInfo mCreatePoolInfo{};

    VkCommandPool mCommandPool{ VK_NULL_HANDLE };

public:
    class Creator {
    public:
        Creator(std::shared_ptr<RLogicalDevice> logicalDevice) :
            mObject{std::make_shared<RCommandPool>(logicalDevice)} {
                mObject->preCreate();
        }

        VkCommandPoolCreateInfo &poolCreateInfo() {
            return mObject->poolCreateInfo();
        }

        std::shared_ptr<RCommandPool> create() {
            mObject->create();
            return mObject;
        }

    private:
        std::shared_ptr<RCommandPool> mObject;
    };
};

}

#endif
