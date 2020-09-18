#pragma once

#include "LogicalDevice.h"
#include "VulkanInstance.h"
#include "Loader.h"
#include "Surface.h"
#include "PhysicalDeviceSelector.h"

namespace Spock::vkCore
{
	class LogicalDeviceFactory
	{
	public:
		virtual std::shared_ptr<LogicalDevice> CreateLogicalVulkanDevice(const VulkanInstance& vulkanInstance, const std::vector<const char*>& extensions, const PhysicalDeviceSelector& deviceSelector, const Surface* surface = nullptr) = 0;
	};

	class LogicalDeviceFactoryImpl : public LogicalDeviceFactory
	{
	public:
		LogicalDeviceFactoryImpl(std::shared_ptr<Loader> loader);
		std::shared_ptr<LogicalDevice> CreateLogicalVulkanDevice(const VulkanInstance& vulkanInstance, const std::vector<const char*>& extensions, const PhysicalDeviceSelector& deviceSelector, const Surface* surface = nullptr) override;
	private:
		std::shared_ptr<Loader> loader;
		const float defaultQueuePriority = 1.0f;

		QueueFamilyIndices DetermineQueueFamilyIndices(const PhysicalDevice device, const Surface* surface) const;
		VkDevice MakeLogicalDevice(const PhysicalDevice device, const std::vector<const char*>& extensions, const QueueFamilyIndices indices) const;
		std::vector<VkDeviceQueueCreateInfo> MakeDeviceQueueCreateInfos(const QueueFamilyIndices indices) const;
		VkDeviceQueueCreateInfo MakeDeviceQueueCreateInfo(const int index) const;
		VkPhysicalDeviceFeatures MakeDeviceFeatureRequest(const PhysicalDevice device) const;
	};

}

