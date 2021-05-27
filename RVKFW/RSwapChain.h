
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

    RSwapChain(std::weak_ptr<RPhysicalDevice> physicalDevice,
               std::weak_ptr<RLogicalDevice> logicalDevice,
               std::weak_ptr<RSurface> surface,
               std::weak_ptr<RWindow> window) : mPhysicalDevice{physicalDevice},
        mLogicalDevice{logicalDevice}, mSurface{surface}, mWindow{window} {
    }

    ~RSwapChain();

    bool create(uint32_t graphicsFamily, uint32_t presentFamily);

    const char * tag() const override {
        return "RSwapChain";
    }

    VkSwapchainKHR handle() const {
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

    void destroy() override;
    
protected:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat();
    VkPresentModeKHR chooseSwapPresentMode();
    VkExtent2D chooseSwapExtent();

    VkSwapchainKHR mSwapChain{ VK_NULL_HANDLE };

    std::weak_ptr<RPhysicalDevice> mPhysicalDevice;
    std::weak_ptr<RLogicalDevice> mLogicalDevice;
    std::weak_ptr<RSurface> mSurface;
    std::weak_ptr<RWindow> mWindow;

    SwapChainSupportDetails mSwapChainSupportDetails{};

    VkSurfaceFormatKHR mSurfaceFormat{};
    VkPresentModeKHR mPresentMode{};
    VkExtent2D mSwapExtent{};

    std::vector<VkImage> mSwapChainImages;
    std::vector<VkImageView> mSwapChainImageViews;

    std::atomic_bool mCreated{ false };
};

}

#endif

