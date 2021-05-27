
#ifndef RInstance_h
#define RInstance_h

#include <string>
#include <memory>
#include <vector>

#include "RObject.h"

namespace rvkfw {

class RPhysicalDevice;
class RSurface;
class RWindow;

class RVkInstance: public std::enable_shared_from_this<RVkInstance>, public RObject {
public:
    RVkInstance(std::weak_ptr<RWindow> window) : mWindow{window} {
    }

    ~RVkInstance();

    void preCreate() override;

    void create(const std::string &appName);

    bool isCreated() const {
        return mResult == VK_SUCCESS;
    }

    bool isValidationLayerAvailable() const;

    VkInstance handle() {
        return mInstance;
    }

    const char * tag() const override {
        return "RInstance";
    }

    std::shared_ptr<RSurface> surface() {
        return mSurface;
    }

    std::shared_ptr<RPhysicalDevice> physicalDevice() {
        return mPhysicalDevice;
    }

protected:

    VkResult mResult{ VK_NOT_READY };
    VkInstance mInstance{ VK_NULL_HANDLE };
    VkApplicationInfo mAppInfo{};
    VkInstanceCreateInfo mCreateInfo{};

    std::weak_ptr<RWindow> mWindow;

    std::shared_ptr<RPhysicalDevice> mPhysicalDevice;
    std::shared_ptr<RSurface> mSurface;

    std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };

};

}

#endif

