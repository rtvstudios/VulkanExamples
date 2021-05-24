
#ifndef RGraphicsPipeline_h
#define RGraphicsPipeline_h

#include "RObject.h"

namespace rvkfw {

class RLogicalDevice;
class RShaderModule;
class RSwapChain;
class RRenderPass;

class RGraphicsPipeline: public RObject {
public:
    RGraphicsPipeline(std::shared_ptr<RRenderPass> renderPass,
                      std::shared_ptr<RSwapChain> swapChain,
                      std::shared_ptr<RLogicalDevice> logicalDevice) :
        mRenderPass{renderPass}, mSwapChain{swapChain}, mLogicalDevice{logicalDevice} {
    }

    ~RGraphicsPipeline();

    void create(const std::string &vertexShaderFile,
                const std::string &fragmentShaderFile);

    const char * tag() const override {
        return "RGraphicsPipeline";
    }

protected:
    std::weak_ptr<RRenderPass> mRenderPass;
    std::weak_ptr<RSwapChain> mSwapChain;
    std::weak_ptr<RLogicalDevice> mLogicalDevice;

    VkPipelineLayout mPipelineLayout{ VK_NULL_HANDLE };
    VkPipeline mGraphicsPipeline{ VK_NULL_HANDLE };
};

}

#endif
