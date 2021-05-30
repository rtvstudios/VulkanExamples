
#ifndef RDrawHelper_h
#define RDrawHelper_h

#include "RObject.h"

namespace rvkfw {

class RPhysicalDevice;
class RLogicalDevice;
class RSwapChain;
class RCommandBuffer;

class RDrawHelper: public RObject {
public:
    RDrawHelper(std::weak_ptr<RLogicalDevice> logicalDevice,
                    std::weak_ptr<RSwapChain> swapChain) :
        mLogicalDevice{logicalDevice}, mSwapChain{swapChain} {
    }

    ~RDrawHelper();

    void create(int framesInFlight);
    
    void destroy() override;

    const char * tag() const override {
        return "RDrawSyncHelper";
    }

    const std::vector<VkSemaphore> &imageAvailableSemaphores() {
        return mImageAvailableSemaphores;
    }

    const std::vector<VkSemaphore> &renderFinishedSemaphores() {
        return mRenderFinishedSemaphores;
    }

    const std::vector<VkFence> &fences() {
        return mInFlightFences;
    }

    int drawIndex() const {
        return mCurrentFrame;
    }

    void draw(const std::shared_ptr<RCommandBuffer> &commandBuffer,
              std::function<void(uint32_t)> onDrawStart = nullptr) const;

protected:
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RSwapChain> mSwapChain;

    int mMaxFramesInFlight = 2;

    std::vector<VkSemaphore> mImageAvailableSemaphores;
    std::vector<VkSemaphore> mRenderFinishedSemaphores;

    mutable std::vector<VkFence> mInFlightFences;
    mutable std::vector<VkFence> mImagesInFlight;

    mutable size_t mCurrentFrame = 0;

};

}

#endif

