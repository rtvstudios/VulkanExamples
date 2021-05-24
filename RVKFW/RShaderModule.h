

#ifndef RShaderModule_h
#define RShaderModule_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;

class RShaderModule: public RObject {
public:
    RShaderModule() = default;
    ~RShaderModule();

    void create(std::shared_ptr<RLogicalDevice> device, const std::string &shaderFile);

    const char * tag() const override {
        return "RShaderModule";
    }

    VkShaderModule handle() {
        return mModule;
    }

protected:
    std::vector<char> readFile(const std::string& filename);

    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    
    VkShaderModule mModule{ VK_NULL_HANDLE };
};

}

#endif
