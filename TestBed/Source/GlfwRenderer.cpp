#include "GlfwRenderer.h"

#include "Common/Version.h"
#include "Common/Logger/Logger.h"
#include "vkCore/PhysicalDeviceSelector.h"
#include "vkCore/Pipeline/PipelineBuilder.h"
#include "vkCore/Pipeline/RenderPassBuilder.h"
#include "vkCore/Pipeline/SubPassBuilder.h"

namespace Spock::Testbed
{
	using namespace vkCore;
	using namespace Common;

	GlfwRenderer::GlfwRenderer(std::shared_ptr<Loader> loader, GLFWwindow* window)
		: instanceFactory(std::make_unique<VulkanInstanceFactoryImpl>(loader)),
		logicalDeviceFactory(std::make_unique<LogicalDeviceFactoryImpl>(loader)),
		swapChainFactory(std::make_unique<SwapChainFactoryImpl>()),
		shaderModuleFactory(std::make_unique<ShaderModuleFactoryImpl>()),
		window(window) {
		loader->LoadVulkanLibrary();
		loader->LoadExportedFunctions();
		loader->LoadGlobalFunctions();
		loader->DiscoverAvailableExtensions();
	}


	const void GlfwRenderer::Init() {
		std::vector<const char*> deviceExtensions{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
		std::vector<const char*> instanceExtensions;

		instance = instanceFactory->CreateVulkanInstance("TestBed", Version(0, 0, 1));
		surfaceFactory = std::make_unique<SurfaceFactoryImpl>(instance.get());
		WindowParameters params;
		params.hWnd = glfwGetWin32Window(window);
		params.hInstance = GetModuleHandle(nullptr);
		surface = surfaceFactory->CreateSurface(params);
		auto deviceSelector = SwapChainSupportedSelector(surface.get());
		device = logicalDeviceFactory->CreateLogicalVulkanDevice(*instance, deviceExtensions, deviceSelector, surface.get());
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		VkExtent2D desiredExtent;
		desiredExtent.height = height;
		desiredExtent.width = width;
		swapChain = swapChainFactory->CreateSwapChain(device, surface.get(), desiredExtent);
		auto pipeline = CreatePipeline();
	}

	std::unique_ptr<Pipeline> GlfwRenderer::CreatePipeline() {
		PipelineBuilder builder;
		auto vertShader = shaderModuleFactory->CreateShaderModule(device, VERT_SHADER_PATH, "main");
		auto fragShader = shaderModuleFactory->CreateShaderModule(device, FRAG_SHADER_PATH, "main");

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

		auto renderPass = CreateRenderPass();

		builder.VertexShader(vertShader);
		builder.FragmentShader(fragShader);
		builder.VertexInputState(vertexInputInfo);
		builder.Topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false);
		builder.Viewport(0, 0, swapChain->GetExtent());
		builder.Scissor(0, 0, swapChain->GetExtent());
		builder.RasterizerDefault();
		builder.DisableMultisampling();
		builder.ColorBlendAttachmentDefaultOpaque();
		builder.RenderPass(renderPass, 0);

		return builder.Build(device);
	}
	std::shared_ptr<vkCore::RenderPass> GlfwRenderer::CreateRenderPass() {
		RenderPassBuilder renderPassBuilder;
		SubPassBuilder subpassBuilder;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		subpassBuilder.AddColorAttachment(colorAttachmentRef);
		
		renderPassBuilder.AddColorAttachmentDefault(swapChain->GetFormat());
		renderPassBuilder.AddSubpass(subpassBuilder.Build());

		return renderPassBuilder.Build(device);
	}
}