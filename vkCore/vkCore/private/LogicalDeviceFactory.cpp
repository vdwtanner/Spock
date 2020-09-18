#include "..\LogicalDeviceFactory.h"

#include <map>
#include <set>

#include "Common/SpockException.h"
#include "Common/Logger/Logger.h"
#include "Common/Functional/Optional.h"

namespace Spock::vkCore
{
    using namespace Common;

    LogicalDeviceFactoryImpl::LogicalDeviceFactoryImpl(std::shared_ptr<Loader> loader) : loader(loader) {
    }

    std::unique_ptr<LogicalDevice> Spock::vkCore::LogicalDeviceFactoryImpl::CreateLogicalVulkanDevice(const VulkanInstance& vulkanInstance, const std::vector<const char*>& extensions, const PhysicalDeviceSelector& deviceSelector, const Surface* surface) {
        auto physicalDevices = vulkanInstance.EnumeratePhysicalDevices();
        auto physicalDevice = deviceSelector.SelectPhysicalDevice(physicalDevices, extensions);
        auto indices = DetermineQueueFamilyIndices(physicalDevice, surface);
        auto vkDevice = MakeLogicalDevice(physicalDevice, extensions, indices);
        auto device = std::make_unique<LogicalDevice>(vkDevice, extensions, indices);
        loader->LoadDeviceLevelFunctions(device.get());
        loader->LoadDeviceLevelFunctionsFromExtensions(device.get());
        device->InitPostFunctionLoad();
        return device;
    }

    QueueFamilyIndices LogicalDeviceFactoryImpl::DetermineQueueFamilyIndices(const PhysicalDevice device, const Surface* surface) const {
        QueueFamilyIndices indices;
        auto queueFamilyProperties = device.FetchQueueFamilyProperties();

        auto x = Optional<uint32_t>::empty();

        // For now we just check for the first queue family that supports Graphics.
        // In the future this will be expanded for various types of queues, such as compute.
        // It is also possible for a single family to have multiple queues, so that could also be taken into consideration later.
        int index = 0;
        for (auto& properties : queueFamilyProperties) {
            if (properties.queueCount == 0) {
                continue;
            }
            if ((properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) > 0) {
                indices.graphicsFamily = index;
            }
            if (surface != nullptr) {
                VkBool32 presentSupport;
                vkGetPhysicalDeviceSurfaceSupportKHR(device.GetVkPhysicalDeviceHandle(), index, surface->GetVkSurfaceHandle(), &presentSupport);
                if (presentSupport) {
                    indices.presentFamily = index;
                }
            }

            if (indices.IsComplete()) {
                break;
            }
            index++;
        }
        return indices;
    }

    VkDevice LogicalDeviceFactoryImpl::MakeLogicalDevice(const PhysicalDevice device, const std::vector<const char*>& extensions, const QueueFamilyIndices indices) const{
        auto queueCreateInfos = MakeDeviceQueueCreateInfos(indices);
        auto features = MakeDeviceFeatureRequest(device);
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.size() > 0 ? &queueCreateInfos[0] : nullptr;
        createInfo.pEnabledFeatures = &features;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.size() > 0 ? &extensions[0] : nullptr;

        VkDevice logicalDevice;
        auto result = vkCreateDevice(device.GetVkPhysicalDeviceHandle(), &createInfo, nullptr, &logicalDevice);
        if (result != VkResult::VK_SUCCESS) {
            THROW_EXCEPTION(SpockException, "Failed to create a logical device! Error Code: " + std::to_string(result));
        }
        
        return logicalDevice;
    }

    std::vector<VkDeviceQueueCreateInfo> LogicalDeviceFactoryImpl::MakeDeviceQueueCreateInfos(const QueueFamilyIndices indices) const {
        std::set<uint32_t> uniqueQueueFamilies;
        auto insert = [&uniqueQueueFamilies](uint32_t queueFamily) { uniqueQueueFamilies.insert(queueFamily); };
        indices.graphicsFamily.Apply(insert);
        indices.presentFamily.Apply(insert);

        auto infos = std::vector<VkDeviceQueueCreateInfo>();
        for (auto queueFamily : uniqueQueueFamilies) {
            infos.push_back(MakeDeviceQueueCreateInfo(queueFamily));
        }
        
        return infos;
    }

    VkDeviceQueueCreateInfo LogicalDeviceFactoryImpl::MakeDeviceQueueCreateInfo(int index) const {
        VkDeviceQueueCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = index;
        info.queueCount = 1;
        info.pQueuePriorities = &defaultQueuePriority;

        return info;
    }

    VkPhysicalDeviceFeatures LogicalDeviceFactoryImpl::MakeDeviceFeatureRequest(const PhysicalDevice device) const {
        //No special features needed at this time
        VkPhysicalDeviceFeatures features{};
        return features;
    }
}
