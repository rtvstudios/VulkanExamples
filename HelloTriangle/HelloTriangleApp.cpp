
#include "HelloTriangleApp.h"

HelloTriangle::HelloTriangle() {
}

HelloTriangle::~HelloTriangle() {
}


void HelloTriangle::init() {
    rvkfw::RApplication::init();

    mRenderPass = std::make_shared<rvkfw::RRenderPass>(logicalDevice(), swapChain());
    mRenderPass->create();

    mFrameBuffer = std::make_shared<rvkfw::RFramebuffer>(mRenderPass, swapChain(), logicalDevice());
    mFrameBuffer->create();

    mGraphicsPipeline = std::make_shared<rvkfw::RGraphicsPipeline>(logicalDevice(), mRenderPass, swapChain());
    mGraphicsPipeline->create(mShaderDirectory + "/ColoredTriangle.vert.spv",
                              mShaderDirectory + "/ColoredTriangle.frag.spv");
    
    mCommandBuffer = std::make_shared<rvkfw::RCommandBuffer>(mRenderPass, swapChain(),
                                                             logicalDevice(), mFrameBuffer, mGraphicsPipeline,
                                                             commandPool());
    mCommandBuffer->create();

    recordDrawCommands();
}

void HelloTriangle::recordDrawCommands() {

}

void HelloTriangle::cleanup() {
    mCommandBuffer = nullptr;
    mGraphicsPipeline = nullptr;
    mCommandBuffer = nullptr;
    mRenderPass = nullptr;
    mFrameBuffer = nullptr;

    rvkfw::RApplication::cleanup();
}

void HelloTriangle::draw() const {

}
