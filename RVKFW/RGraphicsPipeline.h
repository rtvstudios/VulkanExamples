
#ifndef RGraphicsPipeline_h
#define RGraphicsPipeline_h

#include <memory>

namespace rvkfw {

class RLogicalDevice;
class RShaderModule;

class RGraphicsPipeline {
public:
    RGraphicsPipeline() = default;
    ~RGraphicsPipeline();

    void create(std::shared_ptr<RLogicalDevice> device,
                const std::string &vertexShaderFile,
                const std::string &fragmentShaderFile);

protected:
    std::shared_ptr<RShaderModule> mVertexShader;
    std::shared_ptr<RShaderModule> mFragmentShader;

    std::weak_ptr<RLogicalDevice> mLogicalDevice;
};

}

#endif
