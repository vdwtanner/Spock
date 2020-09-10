#pragma once

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#elif __linux
//TODO How to choose proper define? XLIB vs XCB
#endif


#define VK_NO_PROTOTYPES
#include "vkCore/Include/vulkan.h"

namespace Spock::vkCore
{
#define EXPORTED_VULKAN_FUNCTION(name) extern PFN_##name name;
#define GLOBAL_LEVEL_VULKAN_FUNCTION(name) extern PFN_##name name;
#define INSTANCE_LEVEL_VULKAN_FUNCTION(name) extern PFN_##name name;
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) extern PFN_##name name;
#define DEVICE_LEVEL_VULKAN_FUNCTION(name) extern PFN_##name name;
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) extern PFN_##name name;

#include "ListOfVulkanFunctions.inl"
}