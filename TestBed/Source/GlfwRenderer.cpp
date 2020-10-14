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

	GlfwRenderer::~GlfwRenderer() {
		auto deviceHandle = device->GetVkDeviceHandle();
		vkDeviceWaitIdle(deviceHandle);
		vkDestroySemaphore(deviceHandle, imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(deviceHandle, renderFinishedSemaphore, nullptr);
		vkDestroyCommandPool(deviceHandle, commandPool, nullptr);
		for (auto frameBuffer : frameBuffers) {
			vkDestroyFramebuffer(deviceHandle, frameBuffer, nullptr);
		}
	}


	const void GlfwRenderer::Init() {
		instance = instanceFactory->CreateVulkanInstance("TestBed", Version(0, 0, 1));
		CreateSurface();
		CreateDevice();
		CreateSwapChain();
		CreateRenderPass();
		CreatePipeline();
		CreateFrameBuffers();
		CreateCommandPool();
		CreateCommandBuffers();
		RecordCommands();
		CreateSemaphores();
	}

	void GlfwRenderer::RenderFrame() {
		auto imageIndex = AquireImage();
		SubmitDrawCommandBuffer(imageIndex);
		Present(imageIndex);

		vkQueueWaitIdle(device->GetPresentQueue());
	}

	void GlfwRenderer::CreateSurface() {
		surfaceFactory = std::make_unique<SurfaceFactoryImpl>(instance.get());
		WindowParameters params;
		params.hWnd = glfwGetWin32Window(window);
		params.hInstance = GetModuleHandle(nullptr);
		surface = surfaceFactory->CreateSurface(params);
	}

	void GlfwRenderer::CreateDevice() {
		std::vector<const char*> deviceExtensions{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
		auto deviceSelector = SwapChainSupportedSelector(surface.get());
		device = logicalDeviceFactory->CreateLogicalVulkanDevice(*instance, deviceExtensions, deviceSelector, surface.get());
	}

	void GlfwRenderer::CreateSwapChain() {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		VkExtent2D desiredExtent;
		desiredExtent.height = height;
		desiredExtent.width = width;
		swapChain = swapChainFactory->CreateSwapChain(device, surface.get(), desiredExtent);
	}

	void GlfwRenderer::CreateRenderPass() {
		RenderPassBuilder renderPassBuilder;
		SubPassBuilder subpassBuilder;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		subpassBuilder.AddColorAttachment(colorAttachmentRef);

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		renderPassBuilder.AddColorAttachmentDefault(swapChain->GetFormat());
		renderPassBuilder.AddSubpass(subpassBuilder.Build());
		renderPassBuilder.AddSubpassDependency(dependency);

		renderPass = renderPassBuilder.Build(device);
	}

	void GlfwRenderer::CreatePipeline() {
		PipelineBuilder builder;
		auto vertShader = shaderModuleFactory->CreateShaderModule(device, VERT_SHADER_PATH, "main");
		auto fragShader = shaderModuleFactory->CreateShaderModule(device, FRAG_SHADER_PATH, "main");

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

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

		pipeline = builder.Build(device);
	}

	void GlfwRenderer::CreateFrameBuffers() {
		auto numImageViews = swapChain->GetNumImageViews();
		auto extent = swapChain->GetExtent();
		auto deviceHandle = device->GetVkDeviceHandle();
		frameBuffers.resize(numImageViews);
		for (int i = 0; i < numImageViews; i++) {
			VkImageView attachments[] = {
				swapChain->GetImageView(i)
			};

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = renderPass->GetRenderPassHandle();
			createInfo.attachmentCount = 1;
			createInfo.pAttachments = attachments;
			createInfo.width = extent.width;
			createInfo.height = extent.height;
			createInfo.layers = 1;

			VK_CALL(vkCreateFramebuffer(deviceHandle, &createInfo, nullptr, &frameBuffers[i]), "Failed to create FrameBuffer");

		}
	}

	void GlfwRenderer::CreateCommandPool() {
		VkCommandPoolCreateInfo commandPoolCreateInfo{};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = device->GetQueueFamilyIndices().graphicsFamily.Get();
		auto deviceHandle = device->GetVkDeviceHandle();

		VK_CALL(vkCreateCommandPool(deviceHandle, &commandPoolCreateInfo, nullptr, &commandPool), "Failed to create CommandPool");
	}

	void GlfwRenderer::CreateCommandBuffers() {
		int numBuffers = swapChain->GetNumImageViews();
		commandBuffers.resize(numBuffers);
		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = commandPool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = numBuffers;
		auto deviceHandle = device->GetVkDeviceHandle();

		VK_CALL(vkAllocateCommandBuffers(deviceHandle, &allocateInfo, commandBuffers.data()), "Failed to allocate Command Buffers");
	}

	void GlfwRenderer::RecordCommands() {
		for (int i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			VK_CALL(vkBeginCommandBuffer(commandBuffers[i], &beginInfo), "Failed to begin recording Command Buffer");

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass->GetRenderPassHandle();
			renderPassInfo.framebuffer = frameBuffers[i];
			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = swapChain->GetExtent();
			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;
			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetPipelineHandle());
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
			vkCmdEndRenderPass(commandBuffers[i]);

			VK_CALL(vkEndCommandBuffer(commandBuffers[i]), "Failure occurred while ending command buffer recording");
		}
	}
	
	void GlfwRenderer::CreateSemaphores() {
		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		auto deviceHandle = device->GetVkDeviceHandle();
		VK_CALL(vkCreateSemaphore(deviceHandle, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphore), "Failed to create image semaphore");
		VK_CALL(vkCreateSemaphore(deviceHandle, &semaphoreCreateInfo, nullptr, &renderFinishedSemaphore), "Failed to create render semaphore");
	}
	uint32_t GlfwRenderer::AquireImage() {
		uint32_t imageIndex;
		auto deviceHandle = device->GetVkDeviceHandle();
		VK_CALL(vkAcquireNextImageKHR(deviceHandle, swapChain->GetSwapChainHandle(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex),
			"Failed to aquire the next imageIndex");

		return imageIndex;
	}

	void GlfwRenderer::SubmitDrawCommandBuffer(uint32_t imageIndex) {
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		auto graphicsQueue = device->GetQueueFamilyIndices().graphicsFamily.Get();

		VK_CALL(vkQueueSubmit(device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit draw command buffer");
	}
	
	void GlfwRenderer::Present(uint32_t imageIndex) {
		VkSemaphore waitSemaphores[] = { renderFinishedSemaphore };
		VkSwapchainKHR swapChains[] = { swapChain->GetSwapChainHandle() };
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = waitSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		VK_CALL(vkQueuePresentKHR(device->GetPresentQueue(), &presentInfo), "Failed to submit present request");

	}
}