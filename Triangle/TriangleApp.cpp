
#include "TriangleApp.h"
#include "RSwapChain.h"
#include "RLogicalDevice.h"
#include "RSwapChain.h"
#include "RQueue.h"
#include "RWindow.h"

#include <array>
#include <glm/glm.hpp>

TriangleApp::TriangleApp() {
}

TriangleApp::~TriangleApp() {
}

bool TriangleApp::create(const std::string &appName) {
    if (!rvkfw::RApplication::create(appName)) {
        return false;
    }

    mRenderPass = rvkfw::RRenderPass::Creator(logicalDevice(), swapChain()).create();

    mFrameBuffer = rvkfw::RFramebuffer::Creator(mRenderPass, swapChain(), logicalDevice()).create();

    auto graphicsPipelineCreator = rvkfw::RGraphicsPipeline::Creator(logicalDevice(), mRenderPass, swapChain());

    graphicsPipelineCreator.inputAssembly().topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    
    graphicsPipelineCreator.rasterizer().cullMode = VK_CULL_MODE_BACK_BIT;
    graphicsPipelineCreator.rasterizer().frontFace = VK_FRONT_FACE_CLOCKWISE;
    graphicsPipelineCreator.rasterizer().polygonMode = VK_POLYGON_MODE_FILL;

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
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}}
    };

    mVerticesCount = vertices.size();
    const void *data = vertices.data();

    mVertexBuffer = rvkfw::RBufferObject::Creator(physicalDevice(), logicalDevice()).
                        create(data, vertices.size() * sizeof(vertices[0]));

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    graphicsPipelineCreator.vertexInputInfo().vertexBindingDescriptionCount = 1;
    graphicsPipelineCreator.vertexInputInfo().vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());

    graphicsPipelineCreator.vertexInputInfo().pVertexBindingDescriptions = &bindingDescription;
    graphicsPipelineCreator.vertexInputInfo().pVertexAttributeDescriptions = attributeDescriptions.data();

    mGraphicsPipeline = graphicsPipelineCreator.create(mShaderDirectory + "/Shader.vert.spv",
                                                       mShaderDirectory + "/Shader.frag.spv");
    
    mCommandBuffer = rvkfw::RCommandBuffer::Creator(mRenderPass, swapChain(),
                                                             logicalDevice(), mFrameBuffer, mGraphicsPipeline,
                                                    commandPool()).create();

    mDrawHelper = rvkfw::RDrawHelper::Creator(logicalDevice(), swapChain(), commandPool()).create(2);

    recordDrawCommands();

    return true;
}

void TriangleApp::recordDrawCommands() {

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

        vkCmdDraw(commandBuffers[i], mVerticesCount, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void TriangleApp::destroy() {
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

    rvkfw::RApplication::destroy();
}

void TriangleApp::draw() const {
    mDrawHelper->draw(mCommandBuffer);
}
