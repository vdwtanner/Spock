#pragma once

#include "VulkanLogicalDevice.h"
#include "VulkanInstance.h"
#include "Loader.h"

namespace Spock::vkCore
{
	class VulkanLogicalDeviceFactory
	{
	public:
		virtual std::unique_ptr<VulkanLogicalDevice> CreateLogicalVulkanDevice(const VulkanInstance& vulkanInstance, const std::vector<const char*>& extensions) = 0;
	};

	class VulkanLogicalDeviceFactoryImpl : public VulkanLogicalDeviceFactory
	{
	public:
		VulkanLogicalDeviceFactoryImpl(std::shared_ptr<Loader> loader);
		std::unique_ptr<VulkanLogicalDevice> CreateLogicalVulkanDevice(const VulkanInstance& vulkanInstance, const std::vector<const char*>& extensions) override;
	private:
		std::shared_ptr<Loader> loader;
		const float defaultQueuePriority = 1.0f;

		VulkanPhysicalDevice PickPhysicalDevice(const std::vector<VulkanPhysicalDevice>& devices, const std::vector<const char*>& extensions) const;
		int RateDeviceSuitability(const VulkanPhysicalDevice& device, const std::vector<const char*>& extensions) const;
		QueueFamilyIndices DetermineQueueFamilyIndices(const VulkanPhysicalDevice device) const;
		VkDevice MakeLogicalDevice(const VulkanPhysicalDevice device, const std::vector<const char*>& extensions, const QueueFamilyIndices indices) const;
		std::vector<VkDeviceQueueCreateInfo> MakeDeviceQueueCreateInfos(const QueueFamilyIndices indices) const;
		VkDeviceQueueCreateInfo MakeDeviceQueueCreateInfo(const int index) const;
		VkPhysicalDeviceFeatures MakeDeviceFeatureRequest(const VulkanPhysicalDevice device) const;
	};

}

