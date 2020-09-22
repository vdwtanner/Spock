#include "../RenderPass.h"

namespace Spock::vkCore
{

	RenderPass::RenderPass(const std::shared_ptr<LogicalDevice> device, const VkRenderPass renderPassHandle)
		: device(device), vkRenderPassHandle(renderPassHandle) {
	}

	RenderPass::~RenderPass() {
		vkDestroyRenderPass(device->GetVkDeviceHandle(), vkRenderPassHandle, nullptr);
	}
	const VkRenderPass RenderPass::GetRenderPassHandle() const {
		return vkRenderPassHandle;
	}
}