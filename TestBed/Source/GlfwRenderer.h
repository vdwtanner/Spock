#pragma once

#include <memory>

#include "GlfwHelper.h"

#include "vkCore/VulkanInstanceFactory.h"
#include "vkCore/LogicalDeviceFactory.h"
#include "vkCore/SurfaceFactory.h"
#include "vkCore/SwapChainFactory.h"
#include "vkCore/Pipeline/ShaderModuleFactory.h"
#include "vkCore/Pipeline/Pipeline.h"
#include "vkCore/Pipeline/RenderPass.h"

namespace Spock::Testbed
{
	class GlfwRenderer
	{
	public:
		GlfwRenderer(std::shared_ptr<vkCore::Loader> loader, GLFWwindow* window);
		const void Init();
	private:
		const std::string VERT_SHADER_PATH = "Resources/Shaders/Compiled/triangle.vert.spv";
		const std::string FRAG_SHADER_PATH = "Resources/Shaders/Compiled/triangle.frag.spv";

		const std::unique_ptr<vkCore::VulkanInstanceFactory> instanceFactory;
		const std::unique_ptr<vkCore::LogicalDeviceFactory> logicalDeviceFactory;
		const std::unique_ptr<vkCore::SwapChainFactory> swapChainFactory;
		const std::unique_ptr<vkCore::ShaderModuleFactory> shaderModuleFactory;
		GLFWwindow* window;

		std::unique_ptr<vkCore::VulkanInstance> instance;
		std::unique_ptr<vkCore::SurfaceFactory> surfaceFactory;
		std::unique_ptr<vkCore::Surface> surface;
		std::shared_ptr<vkCore::LogicalDevice> device;
		std::unique_ptr<vkCore::SwapChain> swapChain;

		std::unique_ptr<vkCore::Pipeline> CreatePipeline();
		std::shared_ptr<vkCore::RenderPass> CreateRenderPass();
	};
}


