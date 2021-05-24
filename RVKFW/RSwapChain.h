
#ifndef VulSwapChain_h
#define VulSwapChain_h

#include "RNonCopyable.h"

#include <string>
#include <vector>
#include <set>
#include <atomic>
#include <GLFW/glfw3.h>

namespace rvkfw {

class RPhysicalDevice;
class RSurface;
class RWindow;
class RLogicalDevice;

class RSwapChain: private RNonCopyable {
public:
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    RSwapChain(RPhysicalDevice *physicalDevice, RLogicalDevice *logicalDevice, RSurface *surface, RWindow *window);

    ~RSwapChain();

    bool create(uint32_t graphicsFamily, uint32_t presentFamily);
    bool destroy();

    const char * tag() const {
        return "RSwapChain";
    }

    VkSwapchainKHR handle() {
        return mSwapChain;
    }

    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

protected:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat();
    VkPresentModeKHR chooseSwapPresentMode();
    VkExtent2D chooseSwapExtent();

    VkSwapchainKHR mSwapChain{ VK_NULL_HANDLE };
    RPhysicalDevice *mPhysicalDevice{ nullptr };
    RLogicalDevice *mLogicalDevice{nullptr};
    RSurface *mSurface{ nullptr };
    RWindow *mWindow{ nullptr };

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

