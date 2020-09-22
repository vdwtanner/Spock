#include "../PipelineBuilder.h"

#include "Common/Logger/Logger.h"

namespace Spock::vkCore
{
    using namespace Common;

    PipelineBuilder* PipelineBuilder::Viewport(float x, float y, float width, float height) {
        VkViewport viewport{};
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        this->viewport = viewport;
        return this;
    }

    PipelineBuilder* PipelineBuilder::Scissor(int32_t offsetX, int32_t offsetY, VkExtent2D extent) {
        VkRect2D scissor{};
        scissor.offset = { offsetX, offsetY };
        scissor.extent = extent;
        this->scissor = scissor;
        return this;
    }

    PipelineBuilder* PipelineBuilder::Rasterizer(VkPipelineRasterizationStateCreateInfo rasterizer) {
        this->rasterizer = rasterizer;
        return this;
    }

    PipelineBuilder* PipelineBuilder::RasterizerDefault() {
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        this->rasterizer = rasterizer;
        return this;
    }

    PipelineBuilder* PipelineBuilder::Multisampling(VkPipelineMultisampleStateCreateInfo multisampling) {
        this->multisampling = multisampling;
        return this;
    }

    PipelineBuilder* PipelineBuilder::DisableMultisampling() {
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.pSampleMask = nullptr;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        this->multisampling = multisampling;
        return this;
    }

    PipelineBuilder* PipelineBuilder::DepthStencilState(VkPipelineDepthStencilStateCreateInfo depthStencilState) {
        this->depthStencilState = depthStencilState;
        return this;
    }

    PipelineBuilder* PipelineBuilder::ColorBlendAttachment(VkPipelineColorBlendAttachmentState colorBlendAttachment) {
        this->colorBlendAttachment = colorBlendAttachment;
        return this;
    }

    PipelineBuilder* PipelineBuilder::ColorBlendAttachmentDefaultOpaque() {
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        this->colorBlendAttachment = colorBlendAttachment;
        return this;
    }

    PipelineBuilder* PipelineBuilder::ColorBlendAttachmentDefaultAlphaBlend() {
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        this->colorBlendAttachment = colorBlendAttachment;
        return this;
    }

    PipelineBuilder* PipelineBuilder::VertexInputState(const VkPipelineVertexInputStateCreateInfo vertexInputState) {
        this->vertexInputState = vertexInputState;
        return this;
    }

    PipelineBuilder* PipelineBuilder::Topology(const VkPrimitiveTopology topology, const bool enablePrimitiveRestart) {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = topology;
        inputAssembly.primitiveRestartEnable = enablePrimitiveRestart ? VK_TRUE : VK_FALSE;
        this->inputAssemblyState = inputAssembly;
        return this;
    }

    PipelineBuilder* Spock::vkCore::PipelineBuilder::VertexShader(std::shared_ptr<ShaderModule> shaderModule) {
        vertexShader = shaderModule;
        return this;
    }

    PipelineBuilder* PipelineBuilder::TessellationShader(std::shared_ptr<ShaderModule> shaderModule) {
        ASSERT_USAGE(false, "Tessellation shaders aren't supported yet");
        tessellationShader = shaderModule;
        return this;
    }

    PipelineBuilder* PipelineBuilder::GeometryShader(std::shared_ptr<ShaderModule> shaderModule) {
        geometryShader = shaderModule;
        return this;
    }

    PipelineBuilder* PipelineBuilder::FragmentShader(std::shared_ptr<ShaderModule> shaderModule) {
        fragmentShader = shaderModule;
        return this;
    }

    PipelineBuilder* PipelineBuilder::BasePipelineHandle(VkPipeline handle) {
        LOG_ASSERT(basePipelineIndex.IsEmpty());
        basePipelineHandle = handle;
        return this;
    }

    PipelineBuilder* PipelineBuilder::BasePipelineIndex(uint32_t index) {
        LOG_ASSERT(basePipelineHandle.IsEmpty());
        basePipelineIndex = index;
        return this;
    }

    PipelineBuilder* PipelineBuilder::RenderPass(std::shared_ptr<vkCore::RenderPass> renderPass, uint32_t subpassIndex) {
        this->renderPass = renderPass;
        this->subpassIndex = subpassIndex;
        return this;
    }

    std::unique_ptr<Pipeline> PipelineBuilder::Build(std::shared_ptr<LogicalDevice> device) {
        Validate();
        auto pipelineLayout = MakePipelineLayout(device);
        auto shaderStages = MakeShaderStages(device);
        auto flags = MakeFlags();
        auto viewportState = MakeViewportState();
        auto colorBlendInfo = MakeColorBlendInfo();

        VkGraphicsPipelineCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.flags = flags;
        createInfo.stageCount = (uint32_t)shaderStages.size();
        createInfo.pStages = shaderStages.size() > 0 ? shaderStages.data() : nullptr;
        createInfo.pVertexInputState = &(vertexInputState.Get());
        createInfo.pInputAssemblyState = &(inputAssemblyState.Get());
        createInfo.pViewportState = &viewportState;
        createInfo.pRasterizationState = &(rasterizer.Get());
        createInfo.pMultisampleState = &(multisampling.Get());
        createInfo.pDepthStencilState = nullptr; // Optional
        createInfo.pColorBlendState = &colorBlendInfo;
        createInfo.pDynamicState = nullptr; // Optional
        createInfo.layout = pipelineLayout;
        createInfo.renderPass = renderPass->GetRenderPassHandle();
        createInfo.subpass = subpassIndex;

        basePipelineHandle.Apply([&createInfo](auto base) {createInfo.basePipelineHandle = base; });
        basePipelineIndex.Apply([&createInfo](auto base) {createInfo.basePipelineIndex = base; });
        
        VkPipeline pipeline;
        auto result = vkCreateGraphicsPipelines(device->GetVkDeviceHandle(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);
        if (result != VkResult::VK_SUCCESS) {
            THROW_EXCEPTION(SpockException, "Failed to create graphics pipeline with error code: " + std::to_string(result));
        }

        return std::make_unique<Pipeline>(device, pipelineLayout, pipeline);
    }

    void PipelineBuilder::Validate() {
        auto assert = []() {ASSERT_USAGE(false, "Required value was empty"); };
        vertexInputState.IfEmpty(assert);
        inputAssemblyState.IfEmpty(assert);
        viewport.IfEmpty(assert);
        scissor.IfEmpty(assert);
        rasterizer.IfEmpty(assert);
        multisampling.IfEmpty(assert);
        colorBlendAttachment.IfEmpty(assert);
        ASSERT_USAGE(renderPass.get() != nullptr, "Required value renderPass was null");
    }

    VkPipelineCreateFlags PipelineBuilder::MakeFlags() {
        VkPipelineCreateFlags flags = 0;
        if (basePipelineHandle.IsPresent() || basePipelineIndex.IsPresent()) {
            flags |= VkPipelineCreateFlagBits::VK_PIPELINE_CREATE_DERIVATIVE_BIT;
        }
        return VkPipelineCreateFlags();
    }

    VkPipelineViewportStateCreateInfo PipelineBuilder::MakeViewportState() {
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &(viewport.Get());
        viewportState.scissorCount = 1;
        viewportState.pScissors = &(scissor.Get());
        return viewportState;
    }

    VkPipelineColorBlendStateCreateInfo PipelineBuilder::MakeColorBlendInfo() {
        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &(colorBlendAttachment.Get());
        return colorBlending;
    }

    VkPipelineLayout PipelineBuilder::MakePipelineLayout(std::shared_ptr<LogicalDevice> device) {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        VkPipelineLayout layout;
        auto result = vkCreatePipelineLayout(device->GetVkDeviceHandle(), &pipelineLayoutInfo, nullptr, &layout);
        if (result != VkResult::VK_SUCCESS) {
            THROW_EXCEPTION(SpockException, "Failed to create Pipeline layout with error code: " + std::to_string(result));
        }
        return layout;
    }

    std::vector<VkPipelineShaderStageCreateInfo> PipelineBuilder::MakeShaderStages(std::shared_ptr<LogicalDevice> device) {
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
        auto appendShaderStage = [&shaderStages](auto shaderStage) {shaderStages.push_back(shaderStage); };
        vertexShader
            .Map<VkPipelineShaderStageCreateInfo>([&](auto shader) {return MakeShaderStage(VK_SHADER_STAGE_VERTEX_BIT, shader); })
            .Apply(appendShaderStage);
        //TODO: Tessellation shader
        geometryShader
            .Map<VkPipelineShaderStageCreateInfo>([&](auto shader) {return MakeShaderStage(VK_SHADER_STAGE_GEOMETRY_BIT, shader); })
            .Apply(appendShaderStage);
        fragmentShader
            .Map<VkPipelineShaderStageCreateInfo>([&](auto shader) {return MakeShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, shader); })
            .Apply(appendShaderStage);

        return shaderStages;
    }

    VkPipelineShaderStageCreateInfo PipelineBuilder::MakeShaderStage(
        VkShaderStageFlagBits stage,
        std::shared_ptr<ShaderModule> shaderModule
    ) const{
        VkPipelineShaderStageCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.stage = stage;
        createInfo.module = shaderModule->GetShaderModuleHandle();
        createInfo.pName = shaderModule->GetEntryPointName().data();
        return createInfo;
    }

}