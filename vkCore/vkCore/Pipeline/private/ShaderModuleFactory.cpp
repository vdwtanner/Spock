#include "../ShaderModuleFactory.h"

#include "Common/FileUtils.h"
#include "..\ShaderModuleFactory.h"

namespace Spock::vkCore
{
	using namespace Common;
	std::shared_ptr<ShaderModule> Spock::vkCore::ShaderModuleFactory::CreateShaderModule(
		const std::shared_ptr<LogicalDevice> device,
		const std::filesystem::path& pathToShader,
		const std::string entryPointName
	) {
		auto data = FileUtils::ReadFile(pathToShader);
		return CreateShaderModule(device, data, entryPointName);
	}

	std::shared_ptr<ShaderModule> ShaderModuleFactory::CreateShaderModule(
		const std::shared_ptr<LogicalDevice> device, 
		const std::vector<char> shaderBytes,
		const std::string entryPointName
	) {
		auto vkShaderModule = MakeVkShaderModule(device->GetVkDeviceHandle(), shaderBytes);
		return std::make_shared<ShaderModule>(device, vkShaderModule, entryPointName);
	}

	VkShaderModule ShaderModuleFactory::MakeVkShaderModule(const VkDevice deviceHandle, std::vector<char> shaderBytes) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderBytes.data());
		createInfo.codeSize = shaderBytes.size();

		VkShaderModule module;
		auto result = vkCreateShaderModule(deviceHandle, &createInfo, nullptr, &module);
		if (result != VkResult::VK_SUCCESS) {
			THROW_EXCEPTION(SpockException, "Failed to create ShaderModule with error code " + std::to_string(result));
		}
		return module;
	}
}