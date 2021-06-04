
#ifndef TriangleApp_h
#define TriangleApp_h

#include "RApplication.h"
#include "RFramebuffer.h"
#include "RRenderPass.h"
#include "RCommandBuffer.h"
#include "RGraphicsPipeline.h"
#include "RBufferObject.h"
#include "RDrawHelper.h"

class TriangleApp: public rvkfw::RApplication {
public:
    TriangleApp();
    ~TriangleApp();

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
    std::shared_ptr<rvkfw::RDrawHelper> mDrawHelper;

    uint32_t mVerticesCount = 3;
};

#endif

