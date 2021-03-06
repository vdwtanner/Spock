#include "../Pipeline.h"

namespace Spock::vkCore
{
	Pipeline::Pipeline(std::shared_ptr<LogicalDevice> device, VkPipelineLayout pipelineLayout, VkPipeline pipeline) 
	: device(device), vkPipelineLayoutHandle(pipelineLayout), vkPipelineHandle(pipeline) {
	}

	Pipeline::~Pipeline() {
		vkDestroyPipeline(device->GetVkDeviceHandle(), vkPipelineHandle, nullptr);
		vkDestroyPipelineLayout(device->GetVkDeviceHandle(), vkPipelineLayoutHandle, nullptr);
	}

	const VkPipelineLayout Pipeline::GetLayoutHandle() const {
		return vkPipelineLayoutHandle;
	}

	const VkPipeline Pipeline::GetPipelineHandle() const {
		return vkPipelineHandle;
	}
}