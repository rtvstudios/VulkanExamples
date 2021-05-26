
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
    void cleanup() override;

private:
    void recordDrawCommands();

    std::string mShaderDirectory{SHADER_DIRECTORY};
    std::shared_ptr<rvkfw::RRenderPass> mRenderPass;
    std::shared_ptr<rvkfw::RFramebuffer> mFrameBuffer;
    std::shared_ptr<rvkfw::RCommandBuffer> mCommandBuffer;
    std::shared_ptr<rvkfw::RGraphicsPipeline> mGraphicsPipeline;
};
