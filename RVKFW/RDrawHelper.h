
#ifndef RDrawHelper_h
#define RDrawHelper_h

#include "RObject.h"

namespace rvkfw {

class RPhysicalDevice;
class RLogicalDevice;
class RSwapChain;
class RCommandBuffer;
class RCommandPool;
class RBufferObject;
class RImageBuffer;

class RDrawHelper: public RObject {
public:
    RDrawHelper(std::weak_ptr<RLogicalDevice> logicalDevice,
                std::weak_ptr<RSwapChain> swapChain,
                std::weak_ptr<RCommandPool> commandPool) :
        mLogicalDevice{logicalDevice}, mSwapChain{swapChain}, mCommandPool{commandPool} {
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

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void copyBuffer(const std::shared_ptr<RBufferObject> &src,
                    std::shared_ptr<RBufferObject> dst,
                    uint32_t size);

    void transitionImageLayout(const std::shared_ptr<RImageBuffer> &image,
                               VkImageLayout oldLayout, VkImageLayout newLayout);
    
protected:
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RSwapChain> mSwapChain;
    std::weak_ptr<RCommandPool> mCommandPool;

    int mMaxFramesInFlight = 2;

    std::vector<VkSemaphore> mImageAvailableSemaphores;
    std::vector<VkSemaphore> mRenderFinishedSemaphores;

    mutable std::vector<VkFence> mInFlightFences;
    mutable std::vector<VkFence> mImagesInFlight;

    mutable size_t mCurrentFrame = 0;

public:
    class Creator {
    public:
        Creator(std::weak_ptr<RLogicalDevice> logicalDevice,
                    std::weak_ptr<RSwapChain> swapChain,
                    std::weak_ptr<RCommandPool> commandPool) :
            mObject{std::make_shared<RDrawHelper>(logicalDevice, swapChain, commandPool)} {
                mObject->preCreate();
        }

        std::shared_ptr<RDrawHelper> create(int framesInFlight) {
            mObject->create(framesInFlight);
            return mObject;
        }

    private:
        std::shared_ptr<RDrawHelper> mObject;
    };

};

}

#endif

