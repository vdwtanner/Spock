#pragma once

#include <string>
#include <memory>

#include "vkCore/LogicalDevice.h"
#include "vkCore/VulkanFunctions.h"

namespace Spock::vkCore
{
	class ShaderModule
	{
	public:
		ShaderModule(std::shared_ptr<LogicalDevice> device, VkShaderModule moduleHandle, std::string entryPointName = "main");
		~ShaderModule();
		const std::string GetEntryPointName() const;
		const VkShaderModule GetShaderModuleHandle() const;
	private:
		std::shared_ptr<LogicalDevice> device;
		std::string entryPointName;
		VkShaderModule vkShaderModuleHandle;
	};
}