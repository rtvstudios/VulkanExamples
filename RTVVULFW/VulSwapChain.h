
#ifndef VulSwapChain_h
#define VulSwapChain_h

#include "NonCopyable.h"

#include <string>
#include <vector>
#include <set>
#include <atomic>
#include <GLFW/glfw3.h>

namespace rtvvulfw {

class VulPhysicalDevice;
class VulSurface;
class Window;
class VulLogicalDevice;

class VulSwapChain: private NonCopyable {
public:
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    VulSwapChain(VulPhysicalDevice *physicalDevice, VulLogicalDevice *logicalDevice, VulSurface *surface, Window *window);

    ~VulSwapChain();

    bool create(uint32_t graphicsFamily, uint32_t presentFamily);
    bool destroy();

    const char * tag() const {
        return "VulSwapChain";
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
    VulPhysicalDevice *mPhysicalDevice{ nullptr };
    VulLogicalDevice *mLogicalDevice{nullptr};
    VulSurface *mSurface{ nullptr };
    Window *mWindow{ nullptr };

    SwapChainSupportDetails mSwapChainSupportDetails;

    VkSurfaceFormatKHR mSurfaceFormat;
    VkPresentModeKHR mPresentMode;
    VkExtent2D mSwapExtent;

    std::vector<VkImage> mSwapChainImages;

    std::atomic_bool mCreated{ false };
};

}

#endif

