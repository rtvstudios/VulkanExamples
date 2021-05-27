
#include "RApplication.h"
#include "RFramebuffer.h"
#include "RRenderPass.h"
#include "RCommandBuffer.h"
#include "RGraphicsPipeline.h"

class HelloTriangle: public rvkfw::RApplication {
public:
    HelloTriangle();
    ~HelloTriangle();

    void init() override;
    void draw() const override;
    void destroy() override;

private:
    void recordDrawCommands();

    std::string mShaderDirectory{SHADER_DIRECTORY};
    std::shared_ptr<rvkfw::RRenderPass> mRenderPass;
    std::shared_ptr<rvkfw::RFramebuffer> mFrameBuffer;
    std::shared_ptr<rvkfw::RCommandBuffer> mCommandBuffer;
    std::shared_ptr<rvkfw::RGraphicsPipeline> mGraphicsPipeline;

    static const int mMaxFramesInFlight = 2;

    std::vector<VkSemaphore> mImageAvailableSemaphores;
    std::vector<VkSemaphore> mRenderFinishedSemaphores;
    
    mutable std::vector<VkFence> mInFlightFences;
    mutable std::vector<VkFence> mImagesInFlight;
    mutable size_t mCurrentFrame = 0;
};
