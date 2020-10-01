#pragma once

#include <memory>

#include "Pipeline.h"
#include "vkCore/VulkanFunctions.h"
#include "Common/Functional/Optional.h"
#include "ShaderModule.h"
#include "RenderPass.h"
#include "vkCore/LogicalDevice.h"

namespace Spock::vkCore
{
	class PipelineBuilder
	{
	public:
		//Fixed Function
		PipelineBuilder* VertexInputState(const VkPipelineVertexInputStateCreateInfo vertexInputState);
		PipelineBuilder* Topology(const VkPrimitiveTopology topology, const bool enablePrimitiveRestart);
		PipelineBuilder* Viewport(float x, float y, float width, float height);
		PipelineBuilder* Viewport(float x, float y, VkExtent2D extent);
		PipelineBuilder* Viewport(VkRect2D rect);
		PipelineBuilder* Scissor(int32_t offsetX, int32_t offsetY, VkExtent2D extent);
		PipelineBuilder* Rasterizer(VkPipelineRasterizationStateCreateInfo rasterizer);
		PipelineBuilder* RasterizerDefault();
		PipelineBuilder* Multisampling(VkPipelineMultisampleStateCreateInfo multisampling);
		PipelineBuilder* DisableMultisampling();
		PipelineBuilder* DepthStencilState(VkPipelineDepthStencilStateCreateInfo depthStencilState);
		PipelineBuilder* ColorBlendAttachment(VkPipelineColorBlendAttachmentState colorBlendAttachment);
		PipelineBuilder* ColorBlendAttachmentDefaultOpaque();
		PipelineBuilder* ColorBlendAttachmentDefaultAlphaBlend();
		
		//programable
		PipelineBuilder* VertexShader(std::shared_ptr<ShaderModule> shaderModule);
		PipelineBuilder* TessellationShader(std::shared_ptr<ShaderModule> shaderModule);
		PipelineBuilder* GeometryShader(std::shared_ptr<ShaderModule> shaderModule);
		PipelineBuilder* FragmentShader(std::shared_ptr<ShaderModule> shaderModule);

		PipelineBuilder* BasePipelineHandle(VkPipeline handle);
		PipelineBuilder* BasePipelineIndex(uint32_t index);

		PipelineBuilder* RenderPass(std::shared_ptr<vkCore::RenderPass> renderPass, uint32_t subpassIndex);

		std::unique_ptr<Pipeline> Build(std::shared_ptr<LogicalDevice> device);

	private:
		Common::Optional<std::shared_ptr<ShaderModule>> vertexShader;
		Common::Optional<std::shared_ptr<ShaderModule>> tessellationShader;
		Common::Optional<std::shared_ptr<ShaderModule>> geometryShader;
		Common::Optional<std::shared_ptr<ShaderModule>> fragmentShader;

		Common::Optional<VkPipelineVertexInputStateCreateInfo> vertexInputState;
		Common::Optional<VkPipelineInputAssemblyStateCreateInfo> inputAssemblyState;
		Common::Optional<VkViewport> viewport;
		Common::Optional<VkRect2D> scissor;
		Common::Optional<VkPipelineRasterizationStateCreateInfo> rasterizer;
		Common::Optional<VkPipelineMultisampleStateCreateInfo> multisampling;
		Common::Optional<VkPipelineDepthStencilStateCreateInfo> depthStencilState;
		Common::Optional<VkPipelineColorBlendAttachmentState> colorBlendAttachment;
		
		Common::Optional<VkPipeline> basePipelineHandle;
		Common::Optional<uint32_t> basePipelineIndex;
		
		std::shared_ptr<vkCore::RenderPass> renderPass;
		uint32_t subpassIndex;

		void Validate();
		VkPipelineLayout MakePipelineLayout(std::shared_ptr<LogicalDevice> device);
		std::vector<VkPipelineShaderStageCreateInfo> MakeShaderStages(std::shared_ptr<LogicalDevice> device);
		VkPipelineShaderStageCreateInfo MakeShaderStage(VkShaderStageFlagBits stage, std::shared_ptr<ShaderModule> shaderModule) const;
		VkPipelineCreateFlags MakeFlags();
		VkPipelineViewportStateCreateInfo MakeViewportState();
		VkPipelineColorBlendStateCreateInfo MakeColorBlendInfo();
	};
}