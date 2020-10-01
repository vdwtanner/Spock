#pragma once

#include <memory>
#include <filesystem>
#include <vector>

#include "ShaderModule.h"
#include "vkCore/LogicalDevice.h"

namespace Spock::vkCore
{
	class ShaderModuleFactory
	{
	public:
		virtual std::shared_ptr<ShaderModule> CreateShaderModule(const std::shared_ptr<LogicalDevice> device, const std::filesystem::path& pathToShader, const std::string entryPointName) = 0;
		virtual std::shared_ptr<ShaderModule> CreateShaderModule(const std::shared_ptr<LogicalDevice> device, const std::vector<char> shaderBytes, const std::string entryPointName) = 0;
	};

	class ShaderModuleFactoryImpl : public ShaderModuleFactory
	{
	public:
		std::shared_ptr<ShaderModule> CreateShaderModule(const std::shared_ptr<LogicalDevice> device, const std::filesystem::path& pathToShader, const std::string entryPointName) override;
		std::shared_ptr<ShaderModule> CreateShaderModule(const std::shared_ptr<LogicalDevice> device, const std::vector<char> shaderBytes, const std::string entryPointName) override;

	private:
		VkShaderModule MakeVkShaderModule(const VkDevice deviceHandle, std::vector<char> shaderBytes);
	};
}