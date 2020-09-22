#include "../ShaderModule.h"

Spock::vkCore::ShaderModule::ShaderModule(std::shared_ptr<LogicalDevice> device, VkShaderModule moduleHandle, std::string entryPointName)
: device(device), vkShaderModuleHandle(moduleHandle), entryPointName(entryPointName){
}

Spock::vkCore::ShaderModule::~ShaderModule() {
	vkDestroyShaderModule(device->GetVkDeviceHandle(), vkShaderModuleHandle, nullptr);
}

const std::string Spock::vkCore::ShaderModule::GetEntryPointName() const {
	return entryPointName;
}

const VkShaderModule Spock::vkCore::ShaderModule::GetShaderModuleHandle() const {
	return vkShaderModuleHandle;
}
