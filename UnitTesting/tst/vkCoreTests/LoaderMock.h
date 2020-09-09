#pragma once
#include "CommonTestHeaders.h"
#include "vkCore/Loader.h"

namespace UnitTesting::vkCore
{
	class LoaderMock : public Spock::vkCore::Loader
	{
	public:
		MOCK_METHOD(void, LoadVulkanLibrary, (), (override));
		MOCK_METHOD(void, LoadExportedFunctions, (), (override));
		MOCK_METHOD(void, LoadGlobalFunctions, (), (override));
		MOCK_METHOD(void, DiscoverAvailableExtensions, (), (override));
		MOCK_METHOD(void, LoadInstanceLevelFunctions, (const Spock::vkCore::VulkanInstance* instance), (override));
		MOCK_METHOD(void, LoadInstanceLevelFunctionsFromExtensions, (const Spock::vkCore::VulkanInstance* instance), (override));
		MOCK_METHOD(void, LoadDeviceLevelFunctions, (const Spock::vkCore::VulkanLogicalDevice* device), (override));
		MOCK_METHOD(void, LoadDeviceLevelFunctionsFromExtensions, (const Spock::vkCore::VulkanLogicalDevice* device), (override));
		MOCK_METHOD(bool, AreAllExtensionsAvailable, (const std::vector<const char*>& desiredExtensions), (const, override));
	};
}