#include "../SubPassBuilder.h"

#include "Common/Logger/Logger.h"

namespace Spock::vkCore
{
	SubPassBuilder* SubPassBuilder::Flags(VkSubpassDescriptionFlags flags) {
		this->flags = flags;
		return this;
	}

	SubPassBuilder* SubPassBuilder::AddInputAttachment(VkAttachmentReference inputAttachment) {
		inputAttachments.push_back(inputAttachment);
		return this;
	}

	SubPassBuilder* SubPassBuilder::AddColorAttachment(VkAttachmentReference colorAttachment) {
		colorAttachments.push_back(colorAttachment);
		return this;
	}

	VkSubpassDescription SubPassBuilder::Build() {
		VkSubpassDescription subpass{};
		flags.Apply([&subpass](auto fs) {subpass.flags = fs; });
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //only supported bind point for subpasses right now
		subpass.inputAttachmentCount = (uint32_t)inputAttachments.size();
		subpass.pInputAttachments = inputAttachments.size() > 0 ? inputAttachments.data() : nullptr;
		subpass.colorAttachmentCount = (uint32_t)colorAttachments.size();
		subpass.pColorAttachments = colorAttachments.size() > 0 ? colorAttachments.data() : nullptr;
		return subpass;
	}
}