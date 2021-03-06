#include "VulkanRenderPipeline.h"
#include "VulkanDeleter.h"
#include "VulkanDriver.h"
#include "VulkanVertexDeclaration.h"
#include "VulkanGpuPipelineParamsInfo.h"
#include "VulkanShader.h"
#include "../../Device.h"
#include "../DepthStencilState.h"
#include "../BlendingState.h"
#include "../RasterizationState.h"
#include "VulkanVertexDeclaration.h"


namespace sh
{
	namespace video	
	{
		VulkanRenderPipeline::VulkanRenderPipeline()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////

		VulkanRenderPipeline::~VulkanRenderPipeline()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			//vkDestroyPipeline(device, m_graphicsPipeline, nullptr);

			for(auto pipeline : m_intenalPipelines)
			{
				vkDestroyPipeline(device, pipeline, nullptr);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////

		void VulkanRenderPipeline::Load(const pugi::xml_node &node)
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////

		void VulkanRenderPipeline::Unload()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////

		VkPipeline VulkanRenderPipeline::GetVulkanPipeline(const VertexInputDeclarationPtr& vertexDeclaration)
		{
			VulkanVertexDeclaration* vulkanVertexDeclaration = static_cast<VulkanVertexDeclaration*>(vertexDeclaration.get());
			return m_intenalPipelines[vulkanVertexDeclaration->GetId()];
		}

		///////////////////////////////////////////////////////////////////////////////////////
		
		void VulkanRenderPipeline::Init(const VertexInputDeclarationPtr& declaration)
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();
			VkRenderPass renderPass = driver->GetRenderPass();

			// Get vertex input declaration
			VulkanVertexDeclaration* vertexDeclaration = static_cast<VulkanVertexDeclaration*>(declaration.get());
			
			auto it = std::find(m_declarations.begin(), m_declarations.end(), vertexDeclaration);
			if (it != m_declarations.end())
				return;

			vertexDeclaration->m_id = m_declarations.size();
			m_declarations.push_back(vertexDeclaration);

			const auto& inputAttributeDescriptions = vertexDeclaration->GetVulkanAttributes();
			const auto& inputBindingDescription = vertexDeclaration->GetBindingDescription();

			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.vertexAttributeDescriptionCount = inputAttributeDescriptions.size();
			vertexInputInfo.pVertexBindingDescriptions = &inputBindingDescription;
			vertexInputInfo.pVertexAttributeDescriptions = inputAttributeDescriptions.data();

			VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			math::Vector4u vp = driver->GetViewPort();
			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)vp.z;
			viewport.height = (float)vp.w;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent.width = vp.z;
			scissor.extent.height = vp.w;

			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			VkPipelineRasterizationStateCreateInfo rasterizer = {};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = s_vkPolygoneMode[m_description.rasterizationState->fillMode];
			rasterizer.lineWidth = m_description.rasterizationState->lineWidth;
			rasterizer.cullMode = s_vkCullFace[m_description.rasterizationState->cullFace];
			rasterizer.frontFace = s_vkFrontFace[m_description.rasterizationState->frontFace];
			rasterizer.depthBiasEnable = VK_FALSE;

			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			VkPipelineDepthStencilStateCreateInfo depthStencil = {};
			depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencil.depthTestEnable = m_description.depthStencilState->enableDepthTest ? VK_TRUE : VK_FALSE;
			depthStencil.depthWriteEnable = VK_TRUE;
			depthStencil.depthCompareOp = s_vkCompareFunction[m_description.depthStencilState->depthCompareFunction];
			depthStencil.depthBoundsTestEnable = VK_FALSE;
			depthStencil.stencilTestEnable = m_description.depthStencilState->enableStencilTest ? VK_TRUE : VK_FALSE;

			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = m_description.blendingState->enabled ? VK_TRUE : VK_FALSE;
			if(m_description.blendingState->enabled)
			{
				colorBlendAttachment.srcAlphaBlendFactor = s_vkBlendFactor[m_description.blendingState->srcAlpha];
				colorBlendAttachment.dstAlphaBlendFactor = s_vkBlendFactor[m_description.blendingState->dstAlpha];
				colorBlendAttachment.alphaBlendOp = s_vkBlendOperation[m_description.blendingState->operationAlpha];

				colorBlendAttachment.srcColorBlendFactor = s_vkBlendFactor[m_description.blendingState->srcColor];
				colorBlendAttachment.dstColorBlendFactor = s_vkBlendFactor[m_description.blendingState->dstColor];
				colorBlendAttachment.colorBlendOp = s_vkBlendOperation[m_description.blendingState->operationColor];
			}

			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;
		

			VulkanGpuPipelineParamsInfo* paramsInfo = static_cast<VulkanGpuPipelineParamsInfo*>(m_paramsInfo.get());
			VulkanGpuPipelineParamsInfo* autoParamsInfo = static_cast<VulkanGpuPipelineParamsInfo*>(m_autoParamsInfo.get());

			std::vector<VkPushConstantRange> pushConstantRanges;
			std::vector<VkDescriptorSetLayout> setLayouts;
			pushConstantRanges.insert(pushConstantRanges.end(), paramsInfo->GetPushConstantRanges().begin(), paramsInfo->GetPushConstantRanges().end());
			pushConstantRanges.insert(pushConstantRanges.end(), autoParamsInfo->GetPushConstantRanges().begin(), autoParamsInfo->GetPushConstantRanges().end());

			setLayouts.insert(setLayouts.end(), paramsInfo->GetDescriptorSetLayouts().begin(), paramsInfo->GetDescriptorSetLayouts().end());
			setLayouts.insert(setLayouts.end(), autoParamsInfo->GetDescriptorSetLayouts().begin(), autoParamsInfo->GetDescriptorSetLayouts().end());

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};			
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.pushConstantRangeCount = pushConstantRanges.size();
			pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();
			pipelineLayoutInfo.setLayoutCount = setLayouts.size();
			pipelineLayoutInfo.pSetLayouts = setLayouts.data();
			pipelineLayoutInfo.flags = 0;
			pipelineLayoutInfo.pNext = nullptr;		
			

			VkResult res = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create pipeline layout!");


			
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			shaderStages.reserve(6U);
			if (m_description.vertexShader)
			{
				VulkanShader* shader = static_cast<VulkanShader*>(m_description.vertexShader.get());
				shaderStages.push_back(shader->GetVulkanId());
			}
			if (m_description.fragmentShader)
			{
				VulkanShader* shader = static_cast<VulkanShader*>(m_description.fragmentShader.get());
				shaderStages.push_back(shader->GetVulkanId());
			}

			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = shaderStages.size();
			pipelineInfo.pStages = shaderStages.data();
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pDepthStencilState = &depthStencil;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = m_pipelineLayout;
			pipelineInfo.renderPass = renderPass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			VkPipeline pipeline = VK_NULL_HANDLE;
			res = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create graphics pipeline!");

			m_intenalPipelines.push_back(pipeline);
		}
		
		///////////////////////////////////////////////////////////////////////////////////////

		VulkanRenderPipeline::VulkanRenderPipeline(const RenderPipelineDescription& description)
		{
			m_description = description;
		}

		///////////////////////////////////////////////////////////////////////////////////////
	}
}