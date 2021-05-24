
#include "RShaderModule.h"
#include "RLogger.h"
#include "RLogicalDevice.h"
#include "RCreator.h"

#include <fstream>

namespace rvkfw {

void RShaderModule::create(std::shared_ptr<RLogicalDevice> device, const std::string &shaderFile) {
    if (mCreated.exchange(true)) {
        return;
    }

    mLogicalDevice = device;

    auto code = readFile(shaderFile);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(device->handle(), &createInfo, nullptr, &mModule) != VK_SUCCESS) {
        LOG_ERROR(tag(), "failed to create shader module!");
        throw std::runtime_error("failed to create shader module!");
    }

    LOG_DEBUG(tag(), "Shader module is created");
}

RShaderModule::~RShaderModule() {
    if (auto device = mLogicalDevice.lock()) {
        if (mModule != VK_NULL_HANDLE) {
            vkDestroyShaderModule(device->handle(), mModule, nullptr);
        }
    }
}

std::vector<char> RShaderModule::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        LOG_ERROR(tag(), "Failed to open shader file : " << filename);
        throw std::runtime_error(std::string("Failed to open shader file : ") + filename);
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

}
