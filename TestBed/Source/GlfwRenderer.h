#pragma once

#include <memory>

#include "GlfwHelper.h"

#include "vkCore/VulkanInstanceFactory.h"
#include "vkCore/LogicalDeviceFactory.h"
#include "vkCore/SurfaceFactory.h"

namespace Spock::Testbed
{
	class GlfwRenderer
	{
	public:
		GlfwRenderer(std::shared_ptr<vkCore::Loader> loader, GLFWwindow* window);
		const void Init();
	private:
		const std::unique_ptr<vkCore::VulkanInstanceFactory> instanceFactory;
		const std::unique_ptr<vkCore::LogicalDeviceFactory> logicalDeviceFactory;
		GLFWwindow* window;

		std::unique_ptr<vkCore::VulkanInstance> instance;
		std::unique_ptr<vkCore::SurfaceFactory> surfaceFactory;
		std::unique_ptr<vkCore::Surface> surface;
		std::unique_ptr<vkCore::LogicalDevice> device;
	};
}


