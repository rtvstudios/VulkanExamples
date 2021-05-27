

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

    const char * tag() const override {
        return "RShaderModule";
    }

    VkShaderModule handle() const {
        return mModule;
    }

protected:
    std::vector<char> readFile(const std::string& filename);

    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    
    VkShaderModule mModule{ VK_NULL_HANDLE };
};

}

#endif
