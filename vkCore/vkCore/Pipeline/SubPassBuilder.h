#pragma once

#include <vector>

#include "vkCore/VulkanFunctions.h"
#include "Common/Functional/Optional.h"

namespace Spock::vkCore
{

	class SubPassBuilder
	{
	public:
		SubPassBuilder* Flags(VkSubpassDescriptionFlags flags);
		SubPassBuilder* AddInputAttachment(VkAttachmentReference inputAttachment);
		SubPassBuilder* AddOutputAttachment(VkAttachmentReference colorAttachment);
		VkSubpassDescription Build();
	private:
		Common::Optional<VkSubpassDescriptionFlags> flags;
		std::vector<VkAttachmentReference> inputAttachments;
		std::vector<VkAttachmentReference> colorAttachments;
	};
}