

#ifndef RShaderModule_h
#define RShaderModule_h

#include "RNonCopyable.h"

#include <string>
#include <vector>
#include <GLFW/glfw3.h>

namespace rvkfw {

class RLogicalDevice;

class RShaderModule: private RNonCopyable {
public:
    RShaderModule() = default;
    ~RShaderModule();

    void create(std::shared_ptr<RLogicalDevice> device, const std::string &shaderFile);

    const char * tag() const {
        return "RShaderModule";
    }

protected:
    std::vector<char> readFile(const std::string& filename);

    std::weak_ptr<RLogicalDevice> mDevice;
    VkShaderModule mModule{ VK_NULL_HANDLE };
};

}

#endif
