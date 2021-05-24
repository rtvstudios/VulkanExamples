
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

class RInstance: public std::enable_shared_from_this<RInstance>, public RObject {
public:
    RInstance() = default;
    ~RInstance();

    void create(const std::string &appName, std::shared_ptr<RWindow> window);

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

    RSurface * surface() {
        return mSurface.get();
    }

protected:

    VkResult mResult{ VK_NOT_READY };
    VkInstance mInstance{ VK_NULL_HANDLE };
    VkApplicationInfo mAppInfo{};
    VkInstanceCreateInfo mCreateInfo{};

    std::shared_ptr<RPhysicalDevice> mPhysicalDevice;
    std::shared_ptr<RSurface> mSurface;

    std::vector<const char*> mValidationLayers = { "VK_LAYER_KHRONOS_validation" };

};

}

#endif

