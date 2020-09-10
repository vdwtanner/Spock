#pragma once

#include <memory>

#include "vkCore/VulkanInstanceFactory.h"
#include "vkCore/VulkanLogicalDeviceFactory.h"
#include "vkCore/Loader.h"

namespace Spock::Testbed
{
	class TestRenderer
	{
	public:
		TestRenderer(std::shared_ptr<vkCore::Loader> loader);
		const void Init();
	private:
		const std::unique_ptr<vkCore::VulkanInstanceFactory> instanceFactory;
		const std::unique_ptr<vkCore::VulkanLogicalDeviceFactory> logicalDeviceFactory;
		std::unique_ptr<vkCore::VulkanInstance> instance;
		std::unique_ptr<vkCore::VulkanLogicalDevice> device;
	};
}


