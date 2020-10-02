#pragma once

#include <memory>
#include "vkCore/VulkanFunctions.h"
#include "vkCore/LogicalDevice.h"

namespace Spock::vkCore
{
	class Pipeline
	{
	public:
		Pipeline(std::shared_ptr<LogicalDevice> device, VkPipelineLayout pipelineLayout, VkPipeline pipeline);
		~Pipeline();

		const VkPipelineLayout GetLayoutHandle() const;
		const VkPipeline GetPipelineHandle() const;
	private:
		VkPipelineLayout vkPipelineLayoutHandle;
		VkPipeline vkPipelineHandle;
		std::shared_ptr<LogicalDevice> device;
	};
}