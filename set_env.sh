#!/bin/bash

VULKAN_SDK_PATH=/Users/hcl/VulkanSDK/1.2.176.1

export VK_ICD_FILENAMES=$VULKAN_SDK_PATH/macOS/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=$VULKAN_SDK_PATH/macOS/share/vulkan/explicit_layer.d

echo VULKAN_SDK_PATH=$VULKAN_SDK_PATH
echo VK_ICD_FILENAMES=$VK_ICD_FILENAMES
echo VK_LAYER_PATH=$VK_LAYER_PATH

