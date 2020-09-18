#include "GlfwRenderer.h"

#include "Common/Version.h"
#include "Common/Logger/Logger.h"
#include "vkCore/PhysicalDeviceSelector.h"

namespace Spock::Testbed
{
	using namespace vkCore;
	using namespace Common;

	GlfwRenderer::GlfwRenderer(std::shared_ptr<Loader> loader, GLFWwindow* window)
		: instanceFactory(std::make_unique<VulkanInstanceFactoryImpl>(loader)),
		logicalDeviceFactory(std::make_unique<LogicalDeviceFactoryImpl>(loader)),
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
	}
}