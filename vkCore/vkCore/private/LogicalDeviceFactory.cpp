#include "..\LogicalDeviceFactory.h"

#include <map>
#include "Common/SpockException.h"
#include "Common/Logger/Logger.h"

namespace Spock::vkCore
{
    using namespace Common;

    LogicalDeviceFactoryImpl::LogicalDeviceFactoryImpl(std::shared_ptr<Loader> loader) : loader(loader) {
    }

    std::unique_ptr<LogicalDevice> Spock::vkCore::LogicalDeviceFactoryImpl::CreateLogicalVulkanDevice(const VulkanInstance& vulkanInstance, const std::vector<const char*>& extensions) {
        auto physicalDevices = vulkanInstance.EnumeratePhysicalDevices();
        auto physicalDevice = PickPhysicalDevice(physicalDevices, extensions);
        auto indices = DetermineQueueFamilyIndices(physicalDevice);
        auto vkDevice = MakeLogicalDevice(physicalDevice, extensions, indices);
        auto device = std::make_unique<LogicalDevice>(vkDevice, extensions, indices);
        loader->LoadDeviceLevelFunctions(device.get());
        loader->LoadDeviceLevelFunctionsFromExtensions(device.get());
        device->InitPostFunctionLoad();
        return device;
    }

    PhysicalDevice LogicalDeviceFactoryImpl::PickPhysicalDevice(const std::vector<PhysicalDevice>& devices, const std::vector<const char*>& extensions) const{
        std::multimap<int, PhysicalDevice> candidates;

        for (auto& device : devices) {
            candidates.insert(std::make_pair(RateDeviceSuitability(device, extensions), device));
        }

        if (candidates.rbegin()->first > 0) {
            return candidates.rbegin()->second;
        } else {
            THROW_EXCEPTION(SpockException, "Failed to find a suitable GPU!");
        }
    }

    int LogicalDeviceFactoryImpl::RateDeviceSuitability(const PhysicalDevice& device, const std::vector<const char*>& extensions) const{
        auto properties = device.FetchPhysicalDeviceProperties();
        auto features = device.FetchPhysicalDeviceFeatures();

        if (!device.AreAllExtensionsSupported(extensions)) {
            return 0;
        }

        int score = 0;

        //Dedicated GPU > integrated
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        //larger max texture size is better
        score += properties.limits.maxImageDimension2D;
        
        LOG_INFO(std::string(properties.deviceName) + " Suitability Score: " + std::to_string(score));
        return score;
    }

    QueueFamilyIndices LogicalDeviceFactoryImpl::DetermineQueueFamilyIndices(const PhysicalDevice device) const {
        QueueFamilyIndices indices;
        auto queueFamilyProperties = device.FetchQueueFamilyProperties();

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
        auto infos = std::vector<VkDeviceQueueCreateInfo>();
        infos.push_back(MakeDeviceQueueCreateInfo(indices.graphicsFamily.value()));

        return infos;
    }

    VkDeviceQueueCreateInfo LogicalDeviceFactoryImpl::MakeDeviceQueueCreateInfo(const int index) const {
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
