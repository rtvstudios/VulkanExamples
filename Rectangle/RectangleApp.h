
#ifndef RectangleApp_h
#define RectangleApp_h

#include "RApplication.h"
#include "RFramebuffer.h"
#include "RRenderPass.h"
#include "RCommandBuffer.h"
#include "RGraphicsPipeline.h"
#include "RBufferObject.h"
#include "RDrawHelper.h"

class RectangleApp: public rvkfw::RApplication {
public:
    RectangleApp();
    ~RectangleApp();

    bool create(const std::string &appName) override;
    void draw() const override;
    void destroy() override;

private:
    void recordDrawCommands();

    std::string mShaderDirectory{SHADER_DIRECTORY};
    
    std::shared_ptr<rvkfw::RRenderPass> mRenderPass;
    std::shared_ptr<rvkfw::RFramebuffer> mFrameBuffer;
    std::shared_ptr<rvkfw::RCommandBuffer> mCommandBuffer;
    std::shared_ptr<rvkfw::RGraphicsPipeline> mGraphicsPipeline;
    std::shared_ptr<rvkfw::RBufferObject> mVertexBuffer;
    std::shared_ptr<rvkfw::RBufferObject> mIndexBuffer;
    std::shared_ptr<rvkfw::RDrawHelper> mDrawHelper;

    const std::vector<uint16_t> indices = {0, 1, 2, 0, 2, 3};
};

#endif

