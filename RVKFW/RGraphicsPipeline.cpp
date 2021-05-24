
#include "RGraphicsPipeline.h"
#include "RShaderModule.h"
#include "RLogicalDevice.h"
#include "RCreator.h"

namespace rvkfw {

RGraphicsPipeline::~RGraphicsPipeline() {

}

void RGraphicsPipeline::create(std::shared_ptr<RLogicalDevice> device,
                               const std::string &vertexShaderFile,
                               const std::string &fragmentShaderFile) {

    mVertexShader = RCreator::create<RShaderModule>(device, vertexShaderFile);
    mFragmentShader = RCreator::create<RShaderModule>(device, fragmentShaderFile);
    
}

}
