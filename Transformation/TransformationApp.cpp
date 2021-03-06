
#include "TransformationApp.h"
#include "RSwapChain.h"
#include "RLogicalDevice.h"
#include "RSwapChain.h"
#include "RQueue.h"
#include "RWindow.h"
#include "RLogger.h"

TransformationApp::TransformationApp() {
}

TransformationApp::~TransformationApp() {
}

bool TransformationApp::create(const std::string &appName) {
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
        {{-0.25f, -0.25f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.25f, -0.25f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{0.25f, 0.25f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        {{-0.25f, 0.25f}, {1.0f, 0.0f, 1.0f, 1.0f}}
    };
    
    mVertexBuffer = rvkfw::RBufferObject::Creator(physicalDevice(), logicalDevice()).
                        create(vertices.data(), vertices.size() * sizeof(vertices[0]));

    auto indexBuffer = rvkfw::RBufferObject::Creator(physicalDevice(), logicalDevice());
    indexBuffer.bufferInfo().usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    mIndexBuffer = indexBuffer.create(indices.data(), indices.size() * sizeof(indices[0]));

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    mDescriptorPool = rvkfw::RDescriptorPool::Creator(physicalDevice(), logicalDevice(), swapChain()).
                            create(swapChain()->images().size(),
                                   std::vector<uint32_t>(swapChain()->images().size(), sizeof(Transformations)));

    graphicsPipelineCreator.pipelineLayoutInfo().setLayoutCount = 1;
    graphicsPipelineCreator.pipelineLayoutInfo().pSetLayouts = mDescriptorPool->descriptorSetLayout();

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

    std::srand(std::time(nullptr));

    return true;
}

void TransformationApp::recordDrawCommands() {

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

        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
                                mGraphicsPipeline->pipelineLayout(), 0, 1,
                                &mDescriptorPool->descriptorSets()[i], 0, nullptr);

        vkCmdDrawIndexed(commandBuffers[i], indices.size(), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

void TransformationApp::destroy() {
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
    mDescriptorPool = nullptr;

    rvkfw::RApplication::destroy();
}

float TransformationApp::getRandom(float start, float end) const {
    auto rn = std::rand() % (1000+1);
    float t = rn / 1000.0f;
    return start * (1.0f-t) + end * t;
}

void TransformationApp::updateTransformation(uint32_t imageIndex) {
    float t = (currentWallTimeTickCount() - mStartTime) / mDuration;

    if (t > 1.0f) {
        mStartPos = mEndPos;
        if (mEndPos.y > 0) {
            mEndPos.y = (-0.75f);
        } else {
            mEndPos.y = (0.75f);
        }
        mEndPos.x = getRandom(-0.75f, 0.75f);

        mStartAngle = mEndAngle;
        if (mEndAngle < 1.0f) {
            mEndAngle = 2.0f * glm::pi<float>();
        } else {
            mEndAngle = 0.0f;
        }

        mStartTime = currentWallTimeTickCount();

        t = (currentWallTimeTickCount() - mStartTime) / mDuration;
    }

    auto translation = mStartPos * (1-t) + mEndPos * t;
    auto rotation = mStartAngle * (1-t) + mEndAngle * t;

    float scale;
    {

        float t = (currentWallTimeTickCount() - mScaleStartTime) / mScaleDuration;

        if (t > 1.0f) {
            mStartScale = mEndScale;
            if (mEndScale > 1.0f) {
                mEndScale = 0.5f;
            } else {
                mEndScale = 1.5f;
            }
            mScaleStartTime = currentWallTimeTickCount();
            t = (currentWallTimeTickCount() - mScaleStartTime) / mScaleDuration;
        }

        scale = mStartScale * (1-t) + mEndScale * t;
    }


    mTransformation.translation = glm::translate(translation);
    mTransformation.scale = glm::scale(glm::vec3(scale));
    mTransformation.rotation = glm::rotate(rotation, glm::vec3(0, 0, 1));

    imageIndex = imageIndex % mDescriptorPool->buffersCount();
    auto buffer = mDescriptorPool->buffer(imageIndex);
    buffer->setData(&mTransformation, sizeof(Transformations));
}

void TransformationApp::draw() const {
    mDrawHelper->draw(mCommandBuffer, [this](uint32_t imageIndex) {
        const_cast<TransformationApp *>(this)->updateTransformation(imageIndex);
    });
}
