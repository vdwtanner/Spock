#include "../RenderPassBuilder.h"

namespace Spock::vkCore
{
    using namespace Common;

    RenderPassBuilder* Spock::vkCore::RenderPassBuilder::AddColorAttachmentDefault(VkFormat format) {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        colorAttachements.push_back(colorAttachment);
        return this;
    }

    RenderPassBuilder* RenderPassBuilder::AddSubpass(VkSubpassDescription subpass) {
        subpasses.push_back(subpass);
        return this;
    }

    RenderPassBuilder* RenderPassBuilder::AddSubpassDependency(VkSubpassDependency subpassDependency) {
        subpassDependencies.push_back(subpassDependency);
        return this;
    }

    std::shared_ptr<RenderPass> RenderPassBuilder::Build(const std::shared_ptr<LogicalDevice> device) const {
        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount = (uint32_t)colorAttachements.size();
        createInfo.pAttachments = colorAttachements.size() > 0 ? colorAttachements.data() : nullptr;
        createInfo.subpassCount = (uint32_t)subpasses.size();
        createInfo.pSubpasses = subpasses.size() > 0 ? subpasses.data() : nullptr;
        createInfo.dependencyCount = (uint32_t)subpassDependencies.size();
        createInfo.pDependencies = subpassDependencies.size() > 0 ? subpassDependencies.data() : nullptr;

        VkRenderPass renderPassHandle;
        auto result = vkCreateRenderPass(device->GetVkDeviceHandle(), &createInfo, nullptr, &renderPassHandle);
        if (result != VkResult::VK_SUCCESS) {
            THROW_EXCEPTION(SpockException, "Failed to create RenderPass with error code: " + std::to_string(result));
        }

        return std::make_shared<RenderPass>(device, renderPassHandle);
    }
}