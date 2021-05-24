
#include "RSwapChain.h"
#include "RPhysicalDevice.h"
#include "RInstance.h"
#include "RLogger.h"
#include "RSurface.h"
#include "RWindow.h"
#include "RLogicalDevice.h"

namespace rvkfw {

RSwapChain::RSwapChain(RPhysicalDevice *device,
                           RLogicalDevice *logicalDevice,
                           RSurface *surface, RWindow *window)
    : mPhysicalDevice{ device}, mLogicalDevice{ logicalDevice },
      mSurface{ surface}, mWindow{ window} {
}

RSwapChain::~RSwapChain() {
    destroy();
}

bool RSwapChain::create(uint32_t graphicsFamily, uint32_t presentFamily) {
    if (mCreated.exchange(true)) {
        return mCreated;
    }

    mSwapChainSupportDetails = querySwapChainSupport(mPhysicalDevice->handle(), mSurface->handle());

    mSurfaceFormat = chooseSwapSurfaceFormat();
    mPresentMode = chooseSwapPresentMode();
    mSwapExtent = chooseSwapExtent();

    uint32_t imageCount = mSwapChainSupportDetails.capabilities.minImageCount + 1;
    if (mSwapChainSupportDetails.capabilities.maxImageCount > 0 && imageCount >
        mSwapChainSupportDetails.capabilities.maxImageCount) {
        imageCount = mSwapChainSupportDetails.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mSurface->handle();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = mSurfaceFormat.format;
    createInfo.imageColorSpace = mSurfaceFormat.colorSpace;
    createInfo.imageExtent = mSwapExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (graphicsFamily == presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    } else {
        uint32_t queueFamilyIndices[2] = {graphicsFamily, presentFamily};
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    createInfo.preTransform = mSwapChainSupportDetails.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = mPresentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(mLogicalDevice->handle(), &createInfo, nullptr, &mSwapChain) != VK_SUCCESS) {
        mCreated = false;
        LOG_ERROR(tag(), "Failed to create swap chain!");
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(mLogicalDevice->handle(), mSwapChain, &imageCount, nullptr);
    mSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(mLogicalDevice->handle(), mSwapChain, &imageCount, mSwapChainImages.data());

    LOG_INFO(tag(), "Total Number of ImageView: " << imageCount);

    mSwapChainImageViews.resize(mSwapChainImages.size());
    for (size_t i = 0; i < mSwapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = mSwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = mSurfaceFormat.format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(mLogicalDevice->handle(), &createInfo, nullptr, &mSwapChainImageViews[i]) != VK_SUCCESS) {
            LOG_ERROR(tag(), "Failed to create image views!");
            throw std::runtime_error("failed to create image views!");
        }
    }
}

bool RSwapChain::destroy() {
    for (auto imageView : mSwapChainImageViews) {
        vkDestroyImageView(mLogicalDevice->handle(), imageView, nullptr);
    }

    mSwapChainImages.clear();
    mSwapChainImageViews.clear();

    if (mSwapChain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(mLogicalDevice->handle(), mSwapChain, nullptr);
    }
    mCreated = false;
}

RSwapChain::SwapChainSupportDetails RSwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,
                                                  &presentModeCount,
                                                  details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR RSwapChain::chooseSwapSurfaceFormat() {

    for (const auto& availableFormat : mSwapChainSupportDetails.formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            LOG_INFO(tag(), "SwapSurfaceFormat format: VK_FORMAT_B8G8R8A8_SRGB colorSpace: VK_COLOR_SPACE_SRGB_NONLINEAR_KHR");
            return availableFormat;
        }
    }

    LOG_INFO(tag(), "SwapSurfaceFormat format: " << mSwapChainSupportDetails.formats[0].format <<
                    " colorSpace: " << mSwapChainSupportDetails.formats[0].colorSpace);
    return mSwapChainSupportDetails.formats[0];
}

VkPresentModeKHR RSwapChain::chooseSwapPresentMode() {
    for (const auto& availablePresentMode : mSwapChainSupportDetails.presentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            LOG_INFO(tag(), "Presentation Mode: VK_PRESENT_MODE_MAILBOX_KHR");
            return availablePresentMode;
        }
    }

    LOG_INFO(tag(), "Presentation Mode: VK_PRESENT_MODE_FIFO_KHR");
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RSwapChain::chooseSwapExtent() {
    auto capabilities = mSwapChainSupportDetails.capabilities;
    if (capabilities.currentExtent.width != UINT32_MAX) {
        LOG_INFO(tag(), "chooseSwapExtent using width: " << capabilities.currentExtent.width <<
                        " height: " << capabilities.currentExtent.height);
        return capabilities.currentExtent;
    }
    VkExtent2D actualExtent = {mWindow->height(), mWindow->width()};
    actualExtent.width = std::max(capabilities.minImageExtent.width,
                                  std::min(capabilities.maxImageExtent.width,
                                           actualExtent.width));
    actualExtent.height = std::max(capabilities.minImageExtent.height,
                                   std::min(capabilities.maxImageExtent.height,
                                            actualExtent.height));

    LOG_INFO(tag(), "chooseSwapExtent calculated width: " << actualExtent.width <<
                    " height: " << actualExtent.height);

    return actualExtent;
}


}
