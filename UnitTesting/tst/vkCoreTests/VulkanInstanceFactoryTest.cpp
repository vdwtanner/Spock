#include "CommonTestHeaders.h"
#include "vkCore/VulkanInstanceFactory.h"
#include "vkCoreTests/LoaderMock.h"
#include "Common/SpockException.h"
#include "vkCore/VulkanFunctions.h"

using namespace Spock::vkCore;
using namespace Spock::Common;
using namespace ::testing;


typedef void (*test)(const int);

namespace UnitTesting::vkCore
{
#define Setup() LoaderMock* loader = new LoaderMock(); \
	auto loader_sp = std::shared_ptr<Loader>(loader); \
	auto sut = VulkanInstanceFactoryImpl(loader_sp);

	TEST(VulkanInstanceFactoryImpl, DesiredExtensionsUnavailable) {
		Setup();

		EXPECT_CALL(*loader, AreAllExtensionsAvailable)
			.Times(1)
			.WillOnce(Return(false));

		EXPECT_THROW(sut.CreateVulkanInstance("test", Version(0, 1, 0)), SpockException);
	}

	TEST(VulkanInstanceFactoryImpl, VkCreateInstanceFails) {
		Setup();

		EXPECT_CALL(*loader, AreAllExtensionsAvailable)
			.Times(1)
			.WillOnce(Return(true));
		auto createInstance = [](const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {
			pInstance = new VkInstance();
			return VkResult::VK_ERROR_INITIALIZATION_FAILED;
		};
		Spock::vkCore::vkCreateInstance = createInstance;

		EXPECT_THROW(sut.CreateVulkanInstance("test", Version(0, 1, 0)), SpockException);

		Spock::vkCore::vkCreateInstance = nullptr;
	}

	TEST(VulkanInstanceFactoryImpl, InstanceCreatedSuccessfully) {
		Setup();

		EXPECT_CALL(*loader, AreAllExtensionsAvailable)
			.Times(1)
			.WillOnce(Return(true));
		EXPECT_CALL(*loader, LoadInstanceLevelFunctions)
			.Times(1);
		EXPECT_CALL(*loader, LoadInstanceLevelFunctionsFromExtensions)
			.Times(1);
		auto createInstance = [](const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {
			pInstance = new VkInstance();
			return VkResult::VK_SUCCESS;
		};
		Spock::vkCore::vkCreateInstance = createInstance;
		auto destroyInstance = [](VkInstance, const VkAllocationCallbacks*) {};
		Spock::vkCore::vkDestroyInstance = destroyInstance;

		auto result = sut.CreateVulkanInstance("test", Version(0, 1, 0));
		
		result.release();
		Spock::vkCore::vkCreateInstance = nullptr;
		Spock::vkCore::vkDestroyInstance = nullptr;
	}
}

#undef Setup