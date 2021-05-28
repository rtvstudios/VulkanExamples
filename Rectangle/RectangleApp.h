
#ifndef RectangleApp_h
#define RectangleApp_h

#include "RApplication.h"
#include "RFramebuffer.h"
#include "RRenderPass.h"
#include "RCommandBuffer.h"
#include "RGraphicsPipeline.h"
#include "RVertexBuffer.h"
#include "RDrawSyncHelper.h"

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
    std::shared_ptr<rvkfw::RVertexBuffer> mVertexBuffer;
    std::shared_ptr<rvkfw::RDrawSyncHelper> mDrawHelper;

    uint32_t mVerticesCount;
};

#endif

