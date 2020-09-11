#pragma once

#include <memory>

#include "vkCore/VulkanInstanceFactory.h"
#include "vkCore/LogicalDeviceFactory.h"
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
		const std::unique_ptr<vkCore::LogicalDeviceFactory> logicalDeviceFactory;
		std::unique_ptr<vkCore::VulkanInstance> instance;
		std::unique_ptr<vkCore::LogicalDevice> device;
	};
}


