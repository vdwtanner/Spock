#include "TestRenderer.h"

#include "Common/Version.h"

namespace Spock::Testbed
{
	using namespace vkCore;
	using namespace Common;

	Spock::Testbed::TestRenderer::TestRenderer(std::shared_ptr<Loader> loader)
		: instanceFactory(std::make_unique<VulkanInstanceFactoryImpl>(loader)),
		logicalDeviceFactory(std::make_unique<LogicalDeviceFactoryImpl>(loader)) {
		loader->LoadVulkanLibrary();
		loader->LoadExportedFunctions();
		loader->LoadGlobalFunctions();
		loader->DiscoverAvailableExtensions();
	}

	const void TestRenderer::Init() {
		std::vector<const char*> deviceExtensions = {
			"VK_KHR_swapchain"
		};

		instance = instanceFactory->CreateVulkanInstance("TestBed", Version(0, 0, 1));
		device = logicalDeviceFactory->CreateLogicalVulkanDevice(*instance, deviceExtensions);
	}


}