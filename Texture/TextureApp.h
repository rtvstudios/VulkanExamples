
#ifndef TextureApp_h
#define TextureApp_h

#include "RApplication.h"
#include "RFramebuffer.h"
#include "RRenderPass.h"
#include "RCommandBuffer.h"
#include "RGraphicsPipeline.h"
#include "RBufferObject.h"
#include "RDrawHelper.h"
#include "RDescriptorPool.h"
#include "RImageBuffer.h"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <cstdlib>

class TextureApp: public rvkfw::RApplication {
public:
    TextureApp();
    ~TextureApp();

    bool create(const std::string &appName) override;
    void draw() const override;
    void destroy() override;

private:
    void recordDrawCommands();
    void updateTransformation(uint32_t imageIndex);
    int64_t currentWallTimeTickCount() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    float getRandom(float start, float end) const;

    std::string mShaderDirectory{SHADER_DIRECTORY};
    std::string mResourceDirectory{RESOURCE_DIRECTORY};

    std::shared_ptr<rvkfw::RRenderPass> mRenderPass;
    std::shared_ptr<rvkfw::RFramebuffer> mFrameBuffer;
    std::shared_ptr<rvkfw::RCommandBuffer> mCommandBuffer;
    std::shared_ptr<rvkfw::RGraphicsPipeline> mGraphicsPipeline;
    std::shared_ptr<rvkfw::RBufferObject> mVertexBuffer;
    std::shared_ptr<rvkfw::RBufferObject> mIndexBuffer;
    std::shared_ptr<rvkfw::RDrawHelper> mDrawHelper;
    std::shared_ptr<rvkfw::RDescriptorPool> mDescriptorPool;
    std::shared_ptr<rvkfw::RImageBuffer> mImageBuffer;

    struct Transformations {
        glm::mat4 translation{};
        glm::mat4 scale{};
        glm::mat4 rotation{};

    };

    Transformations mTransformation{};

    glm::vec3 mStartPos{0, 0, 0};
    glm::vec3 mEndPos{0, 0, 0};

    float mStartAngle{0};
    float mEndAngle{0};

    float mStartScale{0};
    float mEndScale{0};

    int64_t mStartTime{0};
    static constexpr float mDuration{5000.0f};

    int64_t mScaleStartTime{0};
    static constexpr float mScaleDuration{mDuration/2.0f};

    const std::vector<uint16_t> indices = {0, 1, 2, 0, 2, 3};
};

#endif

