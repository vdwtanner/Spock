#pragma once

#include <memory>

#include "vkCore/LogicalDevice.h"

namespace Spock::vkCore
{

	class RenderPass
	{
	public:
		RenderPass(const std::shared_ptr<LogicalDevice> device, const VkRenderPass renderPassHandle);
		~RenderPass();
		const VkRenderPass GetRenderPassHandle() const;
	private:
		const std::shared_ptr<LogicalDevice> device;
		const VkRenderPass vkRenderPassHandle;
	};
}