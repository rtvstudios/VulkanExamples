
#include "RectangleApp.h"
#include "RSwapChain.h"
#include "RLogicalDevice.h"
#include "RSwapChain.h"
#include "RQueue.h"
#include "RWindow.h"

#include <array>
#include <glm/glm.hpp>

RectangleApp::RectangleApp() {
}

RectangleApp::~RectangleApp() {
}

bool RectangleApp::create(const std::string &appName) {
    if (!rvkfw::RApplication::create(appName)) {
        return false;
    }

    mRenderPass = std::make_shared<rvkfw::RRenderPass>(logicalDevice(), swapChain());
    mRenderPass->create();

    mFrameBuffer = std::make_shared<rvkfw::RFramebuffer>(mRenderPass, swapChain(), logicalDevice());
    mFrameBuffer->create();

    mGraphicsPipeline = std::make_shared<rvkfw::RGraphicsPipeline>(logicalDevice(), mRenderPass, swapChain());
    mGraphicsPipeline->preCreate();
    
    mGraphicsPipeline->inputAssembly().topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    
    mGraphicsPipeline->rasterizer().cullMode = VK_CULL_MODE_BACK_BIT;
    mGraphicsPipeline->rasterizer().frontFace = VK_FRONT_FACE_CLOCKWISE;
    mGraphicsPipeline->rasterizer().polygonMode = VK_POLYGON_MODE_FILL;

    struct Vertex {
        glm::vec2 pos;
        glm::vec4 color;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {0, 1, 2, 0, 2, 3};

    mIndicesCount = indices.size();
    
    mVertexBuffer = std::make_shared<rvkfw::RVertexBuffer>(physicalDevice(), logicalDevice());

    mVertexBuffer->create(vertices.data(), vertices.size() * sizeof(vertices[0]));

    mIndexBuffer = std::make_shared<rvkfw::RVertexBuffer>(physicalDevice(), logicalDevice());
    mIndexBuffer->create(indices.data(), indices.size() * sizeof(indices[0]));

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    mGraphicsPipeline->vertexInputInfo().vertexBindingDescriptionCount = 1;
    mGraphicsPipeline->vertexInputInfo().vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());

    mGraphicsPipeline->vertexInputInfo().pVertexBindingDescriptions = &bindingDescription;
    mGraphicsPipeline->vertexInputInfo().pVertexAttributeDescriptions = attributeDescriptions.data();

    mGraphicsPipeline->create(mShaderDirectory + "/Shader.vert.spv",
                              mShaderDirectory + "/Shader.frag.spv");
    
    mCommandBuffer = std::make_shared<rvkfw::RCommandBuffer>(mRenderPass, swapChain(),
                                                             logicalDevice(), mFrameBuffer, mGraphicsPipeline,
                                                             commandPool());
    mCommandBuffer->create();

    mDrawHelper = std::make_shared<rvkfw::RDrawSyncHelper>(logicalDevice(), swapChain());
    mDrawHelper->create(2);

    recordDrawCommands();

    return true;
}

void RectangleApp::recordDrawCommands() {

    auto commandBuffers = mCommandBuffer->commandBuffers();
    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;
        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = mRenderPass->handle();
        renderPassInfo.framebuffer = mFrameBuffer->frameBufferHandles()[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain()->swapExtent();

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline->handle());

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline->handle());
        VkBuffer vertexBuffers[] = {mVertexBuffer->handle()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffers[i], mIndexBuffer->handle(), 0, VK_INDEX_TYPE_UINT16);

        vkCmdDrawIndexed(commandBuffers[i], mIndicesCount, 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void RectangleApp::destroy() {
    if (!mCreated) {
        return;
    }

    vkDeviceWaitIdle(logicalDevice()->handle());
    
    mDrawHelper = nullptr;
    mCommandBuffer = nullptr;
    mGraphicsPipeline = nullptr;
    mCommandBuffer = nullptr;
    mRenderPass = nullptr;
    mFrameBuffer = nullptr;
    mVertexBuffer = nullptr;
    mIndexBuffer = nullptr;

    rvkfw::RApplication::destroy();
}

void RectangleApp::draw() const {
    mDrawHelper->draw(mCommandBuffer);
}
