
#include "RGraphicsPipeline.h"
#include "RShaderModule.h"
#include "RLogicalDevice.h"
#include "RCreator.h"
#include "RSwapChain.h"
#include "RLogger.h"
#include "RRenderPass.h"

namespace rvkfw {

RGraphicsPipeline::RGraphicsPipeline(std::shared_ptr<RLogicalDevice> logicalDevice,
                                     std::shared_ptr<RRenderPass> renderPass,
                                     std::shared_ptr<RSwapChain> swapChain) :
    mRenderPass{renderPass}, mSwapChain{swapChain}, mLogicalDevice{logicalDevice} {

        mVertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        mVertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        mVertShaderStageInfo.pName = "main";

        mFragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        mFragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        mFragShaderStageInfo.pName = "main";

        mVertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        mVertexInputInfo.vertexBindingDescriptionCount = 0;
        mVertexInputInfo.vertexAttributeDescriptionCount = 0;

        mInputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        mInputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        mInputAssembly.primitiveRestartEnable = VK_FALSE;

        auto swapChainExtent = swapChain->swapExtent();

        mViewport.x = 0.0f;
        mViewport.y = 0.0f;
        mViewport.width = (float) swapChainExtent.width;
        mViewport.height = (float) swapChainExtent.height;
        mViewport.minDepth = 0.0f;
        mViewport.maxDepth = 1.0f;

        mScissor.offset = {0, 0};
        mScissor.extent = swapChainExtent;

        mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        mViewportState.viewportCount = 1;
        mViewportState.pViewports = &mViewport;
        mViewportState.scissorCount = 1;
        mViewportState.pScissors = &mScissor;

        mRasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        mRasterizer.depthClampEnable = VK_FALSE;
        mRasterizer.rasterizerDiscardEnable = VK_FALSE;
        mRasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        mRasterizer.lineWidth = 1.0f;
        mRasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        mRasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        mRasterizer.depthBiasEnable = VK_FALSE;

        mMultisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        mMultisampling.sampleShadingEnable = VK_FALSE;
        mMultisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        mColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        mColorBlendAttachment.blendEnable = VK_FALSE;

        mColorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        mColorBlending.logicOpEnable = VK_FALSE;
        mColorBlending.logicOp = VK_LOGIC_OP_COPY;
        mColorBlending.attachmentCount = 1;
        mColorBlending.pAttachments = &mColorBlendAttachment;
        mColorBlending.blendConstants[0] = 0.0f;
        mColorBlending.blendConstants[1] = 0.0f;
        mColorBlending.blendConstants[2] = 0.0f;
        mColorBlending.blendConstants[3] = 0.0f;

        mPipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        mPipelineLayoutInfo.setLayoutCount = 0;
        mPipelineLayoutInfo.pushConstantRangeCount = 0;

        mPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        mPipelineInfo.pVertexInputState = &mVertexInputInfo;
        mPipelineInfo.pInputAssemblyState = &mInputAssembly;
        mPipelineInfo.pViewportState = &mViewportState;
        mPipelineInfo.pRasterizationState = &mRasterizer;
        mPipelineInfo.pMultisampleState = &mMultisampling;
        mPipelineInfo.pColorBlendState = &mColorBlending;
        mPipelineInfo.renderPass = renderPass->handle();
        mPipelineInfo.subpass = 0;
        mPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
}

RGraphicsPipeline::~RGraphicsPipeline() {
    if (auto logicalDevice = mLogicalDevice.lock()) {
        if (mGraphicsPipeline != VK_NULL_HANDLE) {
            vkDestroyPipeline(logicalDevice->handle(), mGraphicsPipeline, nullptr);
        }
        
        if (mPipelineLayout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(logicalDevice->handle(), mPipelineLayout, nullptr);
        }
    } else {
        LOG_INFO(tag(), "Logical device already destroyed no cleaning up is performed!");
    }
}

void RGraphicsPipeline::create(const std::string &vertexShaderFile,
                               const std::string &fragmentShaderFile) {

    if (mCreated.exchange(true)) {
        return;
    }

    auto logicalDevice = mLogicalDevice.lock();
    auto renderPass = mRenderPass.lock();
    auto swapChain = mSwapChain.lock();
    if (!logicalDevice || !renderPass || !swapChain) {
        LOG_ERROR(tag(), "Creation failed logicalDevice:" << logicalDevice.get() <<
                  " renderPass:" << renderPass.get() <<
                  " swapChain:" << swapChain.get())
        throw std::runtime_error("Graphics pipeline creation failed, required objects are not available!");
    }

    auto vertexShader = RCreator<RShaderModule>().create(logicalDevice, vertexShaderFile);
    auto fragmentShader = RCreator<RShaderModule>().create(logicalDevice, fragmentShaderFile);

    mVertShaderStageInfo.module = vertexShader->handle();
    mFragShaderStageInfo.module = fragmentShader->handle();

    mShaderStages.push_back(mVertShaderStageInfo);
    mShaderStages.push_back(mFragShaderStageInfo);

    mPipelineInfo.stageCount = static_cast<decltype(mPipelineInfo.stageCount)>(mShaderStages.size());
    mPipelineInfo.pStages = mShaderStages.data();

    if (vkCreatePipelineLayout(logicalDevice->handle(), &mPipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create pipeline layout!");
        throw std::runtime_error("failed to create pipeline layout!");
    }

    mPipelineInfo.layout = mPipelineLayout;

    if (vkCreateGraphicsPipelines(logicalDevice->handle(), VK_NULL_HANDLE, 1,
                                  &mPipelineInfo, nullptr, &mGraphicsPipeline) != VK_SUCCESS) {
        LOG_ERROR(tag(), "Failed to create graphics pipeline!");
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

}
