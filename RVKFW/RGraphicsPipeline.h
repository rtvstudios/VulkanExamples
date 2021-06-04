
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
    RGraphicsPipeline(std::weak_ptr<RLogicalDevice> logicalDevice,
                      std::weak_ptr<RRenderPass> renderPass,
                      std::weak_ptr<RSwapChain> swapChain);
    ~RGraphicsPipeline();

    void create(const std::string &vertexShaderFile,
                const std::string &fragmentShaderFile);
    void destroy() override;
    
    const char * tag() const override {
        return "RGraphicsPipeline";
    }

    VkPipelineShaderStageCreateInfo &vertShaderStageInfo() {
        return mVertShaderStageInfo;
    }

    VkPipelineShaderStageCreateInfo &fragShaderStageInfo() {
        return mFragShaderStageInfo;
    }

    VkPipelineVertexInputStateCreateInfo &vertexInputInfo() {
        return mVertexInputInfo;
    }
    
    VkPipelineInputAssemblyStateCreateInfo &inputAssembly() {
        return mInputAssembly;
    }

    VkViewport &viewport() {
        return mViewport;
    }

    VkRect2D &scissor() {
        return mScissor;
    }

    VkPipelineViewportStateCreateInfo &viewportState() {
        return mViewportState;
    }

    VkPipelineRasterizationStateCreateInfo &rasterizer() {
        return mRasterizer;
    }

    VkPipelineMultisampleStateCreateInfo &multisampling() {
        return mMultisampling;
    }
    
    VkPipelineColorBlendAttachmentState &colorBlendAttachment() {
        return mColorBlendAttachment;
    }

    VkPipelineColorBlendStateCreateInfo &colorBlending() {
        return mColorBlending;
    }

    VkPipelineLayoutCreateInfo &pipelineLayoutInfo() {
        return mPipelineLayoutInfo;
    }

    VkGraphicsPipelineCreateInfo &pipelineInfo() {
        return mPipelineInfo;
    }

    VkPipeline handle() const {
        return mGraphicsPipeline;
    }

    VkPipelineLayout pipelineLayout() const {
        return mPipelineLayout;
    }


protected:
    std::weak_ptr<RRenderPass> mRenderPass;
    std::weak_ptr<RSwapChain> mSwapChain;
    std::weak_ptr<RLogicalDevice> mLogicalDevice;

    VkPipelineLayout mPipelineLayout{ VK_NULL_HANDLE };
    VkPipeline mGraphicsPipeline{ VK_NULL_HANDLE };

    VkPipelineShaderStageCreateInfo mVertShaderStageInfo{};
    VkPipelineShaderStageCreateInfo mFragShaderStageInfo{};
    VkPipelineVertexInputStateCreateInfo mVertexInputInfo{};
    VkPipelineInputAssemblyStateCreateInfo mInputAssembly{};
    VkViewport mViewport{};
    VkRect2D mScissor{};
    VkPipelineViewportStateCreateInfo mViewportState{};
    VkPipelineRasterizationStateCreateInfo mRasterizer{};
    VkPipelineMultisampleStateCreateInfo mMultisampling{};
    VkPipelineColorBlendAttachmentState mColorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo mColorBlending{};
    VkPipelineLayoutCreateInfo mPipelineLayoutInfo{};
    VkGraphicsPipelineCreateInfo mPipelineInfo{};
    std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;

public:
    class Creator {
    public:
        Creator(std::weak_ptr<RLogicalDevice> logicalDevice,
                          std::weak_ptr<RRenderPass> renderPass,
                std::weak_ptr<RSwapChain> swapChain) :
            mObject{std::make_shared<RGraphicsPipeline>(logicalDevice, renderPass, swapChain)} {
                mObject->preCreate();
        }

        std::shared_ptr<RGraphicsPipeline> create(const std::string &vertexShaderFile,
                                                  const std::string &fragmentShaderFile) {
            mObject->create(vertexShaderFile, fragmentShaderFile);
            return mObject;
        }

        VkPipelineShaderStageCreateInfo &vertShaderStageInfo() {
            return mObject->vertShaderStageInfo();
        }

        VkPipelineShaderStageCreateInfo &fragShaderStageInfo() {
            return mObject->fragShaderStageInfo();
        }

        VkPipelineVertexInputStateCreateInfo &vertexInputInfo() {
            return mObject->vertexInputInfo();
        }

        VkPipelineInputAssemblyStateCreateInfo &inputAssembly() {
            return mObject->inputAssembly();
        }

        VkViewport &viewport() {
            return mObject->viewport();
        }

        VkRect2D &scissor() {
            return mObject->scissor();
        }

        VkPipelineViewportStateCreateInfo &viewportState() {
            return mObject->viewportState();
        }

        VkPipelineRasterizationStateCreateInfo &rasterizer() {
            return mObject->rasterizer();
        }

        VkPipelineMultisampleStateCreateInfo &multisampling() {
            return mObject->multisampling();
        }

        VkPipelineColorBlendAttachmentState &colorBlendAttachment() {
            return mObject->colorBlendAttachment();
        }

        VkPipelineColorBlendStateCreateInfo &colorBlending() {
            return mObject->colorBlending();
        }

        VkPipelineLayoutCreateInfo &pipelineLayoutInfo() {
            return mObject->pipelineLayoutInfo();
        }

        VkGraphicsPipelineCreateInfo &pipelineInfo() {
            return mObject->pipelineInfo();
        }

    private:
        std::shared_ptr<RGraphicsPipeline> mObject;
    };
};

}

#endif
