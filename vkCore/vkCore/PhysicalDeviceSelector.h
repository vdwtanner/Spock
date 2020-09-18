#pragma once

#include <vector>

#include "Surface.h"
#include "PhysicalDevice.h"

namespace Spock::vkCore
{
	class PhysicalDeviceSelector
	{
	public:
		virtual PhysicalDevice SelectPhysicalDevice(const std::vector<PhysicalDevice>& devices, const std::vector<const char*>& requiredExtensions) const = 0;
	};

	class SwapChainSupportedSelector : public PhysicalDeviceSelector
	{
	public:
		SwapChainSupportedSelector(const Surface* surface);
		PhysicalDevice SelectPhysicalDevice(const std::vector<PhysicalDevice>& devices, const std::vector<const char*>& requiredExtensions) const override;
	private:
		const Surface* surface;

		int RateDeviceSuitability(const PhysicalDevice& device, const std::vector<const char*>& extensions) const;
		bool IsSwapChainSupported(const PhysicalDevice& device) const;
	};
}