
#ifndef RSwapChain_h
#define RSwapChain_h

#include "RObject.h"

namespace rvkfw {

class RPhysicalDevice;
class RSurface;
class RWindow;
class RLogicalDevice;

class RSwapChain: public RObject {
public:
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    RSwapChain() = default;

    ~RSwapChain();

    bool create(std::shared_ptr<RPhysicalDevice> physicalDevice,
                std::shared_ptr<RLogicalDevice> logicalDevice,
                std::shared_ptr<RSurface> surface,
                std::shared_ptr<RWindow> window,
                uint32_t graphicsFamily, uint32_t presentFamily);

    bool destroy();

    const char * tag() const override {
        return "RSwapChain";
    }

    VkSwapchainKHR handle() {
        return mSwapChain;
    }

    VkExtent2D swapExtent() const {
        return mSwapExtent;
    }

    VkSurfaceFormatKHR surfaceFormat() const {
        return mSurfaceFormat;
    }

    const std::vector<VkImageView> &imageViews() const {
        return mSwapChainImageViews;
    }

    const std::vector<VkImage> &images() const {
        return mSwapChainImages;
    }

    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

protected:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat();
    VkPresentModeKHR chooseSwapPresentMode();
    VkExtent2D chooseSwapExtent();

    VkSwapchainKHR mSwapChain{ VK_NULL_HANDLE };

    std::weak_ptr<RPhysicalDevice> mPhysicalDevice;
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RSurface> mSurface;
    std::weak_ptr<RWindow> mWindow;

    SwapChainSupportDetails mSwapChainSupportDetails;

    VkSurfaceFormatKHR mSurfaceFormat;
    VkPresentModeKHR mPresentMode;
    VkExtent2D mSwapExtent;

    std::vector<VkImage> mSwapChainImages;
    std::vector<VkImageView> mSwapChainImageViews;

    std::atomic_bool mCreated{ false };
};

}

#endif

