#include "../PhysicalDeviceSelector.h"

#include <map>
#include "vkCore/VulkanFunctions.h"
#include "Common/SpockException.h"
#include "Common/Logger/Logger.h"

namespace Spock::vkCore
{
    using namespace Common;

	Spock::vkCore::SwapChainSupportedSelector::SwapChainSupportedSelector(const Surface* surface) : surface(surface) {
	}

	PhysicalDevice SwapChainSupportedSelector::SelectPhysicalDevice(const std::vector<PhysicalDevice>& devices, const std::vector<const char*>& requiredExtensions) const {
        std::multimap<int, PhysicalDevice> candidates;

        for (auto& device : devices) {
            candidates.insert(std::make_pair(RateDeviceSuitability(device, requiredExtensions), device));
        }

        if (candidates.rbegin()->first > 0) {
            return candidates.rbegin()->second;
        } else {
            THROW_EXCEPTION(SpockException, "Failed to find a suitable GPU!");
        }
	}

	int SwapChainSupportedSelector::RateDeviceSuitability(const PhysicalDevice& device, const std::vector<const char*>& requiredExtensions) const {
        auto properties = device.FetchPhysicalDeviceProperties();
        auto features = device.FetchPhysicalDeviceFeatures();

        if (!device.AreAllExtensionsSupported(requiredExtensions)) {
            return 0;
        }

        if (!IsSwapChainSupported(device)) {
            return 0;
        }

        int score = 0;

        //Dedicated GPU > integrated
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        //larger max texture size is better
        score += properties.limits.maxImageDimension2D;

        LOG_INFO(std::string(properties.deviceName) + " Suitability Score: " + std::to_string(score));
        return score;
	}

    bool SwapChainSupportedSelector::IsSwapChainSupported(const PhysicalDevice& device) const {
        // We only need to check that there are multiple Present modes from vkGetPhysicalDeviceSurfacePresentModesKHR
        // as vkGetPhysicalDeviceSurfaceFormatsKHR is required to provide at least 1 supported format according to the spec
        uint32_t numPresentationModes;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            device.GetVkPhysicalDeviceHandle(), surface->GetVkSurfaceHandle(), &numPresentationModes, nullptr);
        return numPresentationModes > 0;
    }
}