#pragma once

#include <vector>
#include <memory>

#include "vkCore/VulkanFunctions.h"
#include "RenderPass.h"
#include "vkCore/LogicalDevice.h"

namespace Spock::vkCore
{
	class RenderPassBuilder
	{
	public:
		RenderPassBuilder* AddColorAttachmentDefault(VkFormat format);
		RenderPassBuilder* AddSubpass(VkSubpassDescription subpass);
		RenderPassBuilder* AddSubpassDependency(VkSubpassDependency subpassDependency);
		std::unique_ptr<RenderPass> Build(const std::shared_ptr<LogicalDevice> device) const;
	private:
		std::vector<VkAttachmentDescription> colorAttachements;
		std::vector<VkSubpassDescription> subpasses;
		std::vector<VkSubpassDependency> subpassDependencies;
	};
}