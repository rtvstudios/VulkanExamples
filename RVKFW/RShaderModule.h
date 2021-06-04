

#ifndef RShaderModule_h
#define RShaderModule_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;

class RShaderModule: public RObject {
public:
    RShaderModule(std::weak_ptr<RLogicalDevice> logicalDevice) :
        mLogicalDevice{logicalDevice} {
    }

    ~RShaderModule();

    void create(const std::string &shaderFile);
    void destroy() override;

    const char * tag() const override {
        return "RShaderModule";
    }

    VkShaderModule handle() const {
        return mModule;
    }

    VkShaderModuleCreateInfo &createInfo() {
        return mCreateInfo;
    }

protected:
    std::vector<char> readFile(const std::string& filename);

    std::weak_ptr<RLogicalDevice> mLogicalDevice;

    VkShaderModuleCreateInfo mCreateInfo{};

    VkShaderModule mModule{ VK_NULL_HANDLE };

public:
    class Creator {
    public:
        Creator(std::weak_ptr<RLogicalDevice> logicalDevice) :
            mObject{std::make_shared<RShaderModule>(logicalDevice)} {
                mObject->preCreate();
        }

        std::shared_ptr<RShaderModule> create(const std::string &shaderFile) {
            mObject->create(shaderFile);
            return mObject;
        }

        VkShaderModuleCreateInfo &createInfo() {
            return mObject->createInfo();
        }

    private:
        std::shared_ptr<RShaderModule> mObject;
    };
};

}

#endif
