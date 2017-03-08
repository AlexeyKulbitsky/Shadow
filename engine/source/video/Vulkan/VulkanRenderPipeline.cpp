#include "VulkanRenderPipeline.h"
#include "VulkanDeleter.h"
#include "VulkanDriver.h"
#include "VulkanShaderProgram.h"
#include "VulkanVertexDeclaration.h"
#include "VulkanUniformBuffer.h"
#include "../../Device.h"
#include "../UniformBuffer.h"
#include "../DepthStencilState.h"
#include "../BlendingState.h"
#include "../RasterizationState.h"
#include "../VertexDeclaration.h"
#include "../ShaderProgram.h"


namespace sh
{
	namespace video	
	{
		VulkanRenderPipeline::VulkanRenderPipeline()
		{
			createRenderPass();
		}

		///////////////////////////////////////////////////////////////////////////////////////

		VulkanRenderPipeline::~VulkanRenderPipeline()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			vkDestroyPipeline(device, m_graphicsPipeline, nullptr);
			vkDestroyRenderPass(device, m_renderPass, nullptr);
		}

		///////////////////////////////////////////////////////////////////////////////////////

		RenderPipelinePtr VulkanRenderPipeline::Clone()
		{
			RenderPipelinePtr result;

			VulkanRenderPipeline* pipeline = new VulkanRenderPipeline();
			pipeline->m_shaderProgram = m_shaderProgram;
			pipeline->m_uniformBuffer = UniformBufferPtr(m_uniformBuffer->Clone());
			pipeline->m_depthStencilState = DepthStencilStatePtr(m_depthStencilState->Clone());
			pipeline->m_blendingState = BlendingStatePtr(m_blendingState->Clone());
			pipeline->m_rasterizationState = RasterizationStatePtr(m_rasterizationState->Clone());
			pipeline->m_vertexInputDeclaration = m_vertexInputDeclaration->Clone();
			pipeline->m_name = m_name;
			pipeline->m_layer = m_layer;

			result.reset(pipeline);
			return result;
		}

		///////////////////////////////////////////////////////////////////////////////////////

		void VulkanRenderPipeline::Load(const pugi::xml_node &node)
		{
			RenderPipeline::LoadStates(node);

			Driver* driver = Device::GetInstance()->GetDriver();


			// Load shader program
			pugi::xml_node shaderProgramNode = node.child("shader");

			while (shaderProgramNode.attribute("api").as_string() != String("Vulkan"))
			{
				shaderProgramNode = shaderProgramNode.next_sibling();
			}
			if (!shaderProgramNode.empty())
			{
				m_shaderProgram = driver->CreateShaderProgram();
				m_shaderProgram->Load(shaderProgramNode);
			}

			// Load attributes
			pugi::xml_node attributesNode = node.child("attributes");
			if (!attributesNode.empty())
			{
				m_vertexInputDeclaration = driver->CreateVertexInputDeclaration();
				m_vertexInputDeclaration->SetShaderProgram(m_shaderProgram.get());
				m_vertexInputDeclaration->Load(attributesNode);
				m_vertexInputDeclaration->Init();
			}

			// Load uniforms
			pugi::xml_node uniformsNode = node.child("constants");
			if (!uniformsNode.empty())
			{
				m_uniformBuffer = driver->CreateUniformBuffer();
				m_uniformBuffer->SetShaderProgram(m_shaderProgram.get());
				m_uniformBuffer->Load(uniformsNode);
				m_uniformBuffer->Init();
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////

		void VulkanRenderPipeline::Unload()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////
		
		void VulkanRenderPipeline::Init()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();
			VulkanShaderProgram* shaderProgram = static_cast<VulkanShaderProgram*>(m_shaderProgram.get());

			const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages = shaderProgram->GetShaderStages();

			

			VKVertexDeclaration* vertexDeclaration = static_cast<VKVertexDeclaration*>(m_vertexInputDeclaration);
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
			rasterizer.polygonMode = s_vkPolygoneMode[static_cast<size_t>(m_rasterizationState->fillMode)];
			rasterizer.lineWidth = m_rasterizationState->lineWidth;
			rasterizer.cullMode = s_vkCullFace[static_cast<size_t>(m_rasterizationState->cullFace)];
			rasterizer.frontFace = s_vkFrontFace[static_cast<size_t>(m_rasterizationState->frontFace)];
			rasterizer.depthBiasEnable = VK_FALSE;

			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			VkPipelineDepthStencilStateCreateInfo depthStencil = {};
			depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencil.depthTestEnable = m_depthStencilState->enableDepthTest ? VK_TRUE : VK_FALSE;
			depthStencil.depthWriteEnable = VK_TRUE;
			depthStencil.depthCompareOp = s_vkCompareFunction[static_cast<size_t>(m_depthStencilState->depthCompareFunction)];
			depthStencil.depthBoundsTestEnable = VK_FALSE;
			depthStencil.stencilTestEnable = m_depthStencilState->enableStencilTest ? VK_TRUE : VK_FALSE;

			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;

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

			VulkanUniformBuffer* uniformBuffer = static_cast<VulkanUniformBuffer*>(m_uniformBuffer.get());
			
			VkDescriptorSetLayout setLayouts[] = { uniformBuffer->m_descriptorSetLayout };
			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 1;
			pipelineLayoutInfo.pSetLayouts = setLayouts;
			pipelineLayoutInfo.pushConstantRangeCount = 0;
			
			SH_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &(uniformBuffer->m_pipelineLayout)) == VK_SUCCESS,
				"failed to create pipeline layout!");

			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages.data();
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pDepthStencilState = &depthStencil;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = uniformBuffer->m_pipelineLayout;
			pipelineInfo.renderPass = m_renderPass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			SH_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) == VK_SUCCESS,
				"failed to create graphics pipeline!");
		}

		///////////////////////////////////////////////////////////////////////////////////////
	
		void VulkanRenderPipeline::createRenderPass()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();


			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = driver->GetSwapChainImageFormat();
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentDescription depthAttachment = {};
			depthAttachment.format = driver->findDepthFormat();
			depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			VkAttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkAttachmentReference depthAttachmentRef = {};
			depthAttachmentRef.attachment = 1;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subPass = {};
			subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subPass.colorAttachmentCount = 1;
			subPass.pColorAttachments = &colorAttachmentRef;
			subPass.pDepthStencilAttachment = &depthAttachmentRef;

			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = attachments.size();
			renderPassInfo.pAttachments = attachments.data();
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subPass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			SH_ASSERT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_renderPass) == VK_SUCCESS,
				"failed to create render pass!");
		}

		///////////////////////////////////////////////////////////////////////////////////////
	}
}