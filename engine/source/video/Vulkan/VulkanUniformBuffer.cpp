#include "VulkanUniformBuffer.h"
#include "../../Device.h"
#include "VulkanDriver.h"

namespace sh
{
	namespace video
	{
		VulkanUniformBuffer::VulkanUniformBuffer()
		{
			createDecriptorPool();
			createDescriptorSet();

			VkDeviceSize bufferSize = 0U;//sizeof(UniformBufferObject);

			createBuffer(bufferSize, 
						 VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
						 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
						 m_uniformStagingBuffer, 
						 m_uniformStagingBufferMemory);
			createBuffer(bufferSize, 
						 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
						 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
						 m_uniformBuffer, 
						 m_uniformBufferMemory);
		}

		/////////////////////////////////////////////////////////////////////////

		VulkanUniformBuffer::~VulkanUniformBuffer()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
			vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
			
			vkFreeMemory(device, m_uniformStagingBufferMemory, nullptr);
			vkDestroyBuffer(device, m_uniformStagingBuffer, nullptr);
			vkFreeMemory(device, m_uniformBufferMemory, nullptr);
			vkDestroyBuffer(device, m_uniformBuffer, nullptr);

			vkDestroyDescriptorPool(device, m_descriptorPool, nullptr);

		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::Load(const pugi::xml_node &node)
		{
			// Load uniforms
			pugi::xml_node uniformsNode = node.child("uniforms");
			LoadUniforms(uniformsNode);

			// Load texure samplers
			pugi::xml_node samplersNode = node.child("samplers");
			LoadSamplers(samplersNode);
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::Init()
		{
			createDescriptorSetLayout();
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::Upload()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			struct UniformBufferObject {};
			UniformBufferObject ubo = {};

			/*
			ubo.model.setIdentity();
			math::quatf rr = math::quatf().setFromAngleAxis(-math::k_pi2, math::vec3f(1.0f, 0.0f, 0.0f));
			math::quatf r = math::quatf().setFromAngleAxis(rot, math::vec3f(0.0f, 0.0f, 1.0f));
			ubo.model.setRotation((rr * r).getAsMat3());

			ubo.view.setIdentity();
			ubo.view.setTranslation(math::vec3f(0.0f, 0.5f, -3.0f));
			
			ubo.proj = math::perspective(math::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
			*/
			void* data;
			vkMapMemory(device, m_uniformStagingBufferMemory, 0, sizeof(ubo), 0, &data);
			memcpy(data, &ubo, sizeof(ubo));
			vkUnmapMemory(device, m_uniformStagingBufferMemory);

			copyBuffer(m_uniformStagingBuffer, m_uniformBuffer, sizeof(ubo));
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::SetShaderProgram(ShaderProgram* shaderProgram)
		{
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::LoadUniforms(const pugi::xml_node &node)
		{
#if 0
			if (node.empty())
				return;

			Uniform* uniform = nullptr;

			pugi::xml_node uniformNode = node.first_child();
			while (!uniformNode.empty())
			{
				pugi::xml_attribute nameAttr = uniformNode.attribute("name");
				pugi::xml_attribute typeAttr = uniformNode.attribute("type");
				pugi::xml_attribute valAttr = uniformNode.attribute("val");

				printf("Uniform:\n");
				std::string name = nameAttr.as_string();
				printf("\tName: %s ", name.c_str());

				std::string typeName = typeAttr.as_string();
				printf("Type: %s ", typeName.c_str());
				if (typeName == "float")
				{
					GLES20UniformFloat* glesUniform = new GLES20UniformFloat(0.0f);
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{
						float value = valAttr.as_float();
						printf("Value: %f", value);
						uniform->Set(value);
					}

				}
				else if (typeName == "int")
				{
					GLES20UniformInt *glesUniform = new GLES20UniformInt(0);
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{
						int value = valAttr.as_int();
						printf("Value: %d", value);
						uniform->Set(value);
					}
				}
				else if (typeName == "vec2")
				{
					GLES20UniformVector2f *glesUniform = new GLES20UniformVector2f(sh::math::Vector2f(0.0f));
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{

					}
				}
				else if (typeName == "vec3")
				{
					GLES20UniformVector3f *glesUniform = new GLES20UniformVector3f(sh::math::Vector3f(0.0f));
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{
					}
				}
				else if (typeName == "vec4")
				{
					GLES20UniformVector4f *glesUniform = new GLES20UniformVector4f(sh::math::Vector4f(0.0f));
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{

					}
				}
				else if (typeName == "mat4")
				{
					GLES20UniformMatrix4f *glesUniform = new GLES20UniformMatrix4f(sh::math::Matrix4f());
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{
						String value = valAttr.as_string();
						if (value == "model.worldMatrix")
						{
							glesUniform->SetGlobalUniformName(GlobalUniformName::MODEL_WORLD_MATRIX);
						}
						else if (value == "model.worldViewProjectionMatrix")
						{
							glesUniform->SetGlobalUniformName(GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX);
						}
						else if (value == "camera.viewProjectionMatrix")
						{
							glesUniform->SetGlobalUniformName(GlobalUniformName::CAMERA_VIEW_PROJECTION_MATRIX);
						}
						else if (value == "camera.viewRotationProjectionMatrix")
						{
							glesUniform->SetGlobalUniformName(GlobalUniformName::CAMERA_VIEW_ROTATION_PROJECTION_MATRIX);
						}
					}
				}
				else if (typeName == "vec3Array")
				{
					GLES20UniformVector3fArray* glesUniform = new GLES20UniformVector3fArray(std::vector<math::Vector3f>(0.0f));
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					String value = valAttr.as_string();
					if (value == "light.directional.direction")
					{
						glesUniform->SetGlobalUniformName(GlobalUniformName::LIGHT_DIRECTIONAL_DIRECTION);
					}
				}

				printf("\n");

				uniform->SetName(name);

				AddUniform(uniform);
				uniform = nullptr;

				uniformNode = uniformNode.next_sibling();
			}
#endif
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::LoadSamplers(const pugi::xml_node &node)
		{
#if 0
			if (node.empty())
				return;

			pugi::xml_node uniformNode = node.first_child();

			GLES20Sampler* sampler = new GLES20Sampler(m_shaderProgram->GetGLId());

			while (!uniformNode.empty())
			{
				pugi::xml_attribute nameAttr = uniformNode.attribute("name");
				pugi::xml_attribute typeAttr = uniformNode.attribute("type");
				pugi::xml_attribute tilingUAttr = uniformNode.attribute("tilingU");
				pugi::xml_attribute tilingVAttr = uniformNode.attribute("tilingV");
				pugi::xml_attribute tilingWAttr = uniformNode.attribute("tilingW");
				pugi::xml_attribute filerAttr = uniformNode.attribute("filter");
				pugi::xml_attribute usageAttr = uniformNode.attribute("usage");

				printf("Sampler:\n");
				std::string name = nameAttr.as_string();
				printf("\tName: %s ", name.c_str());

				// Read texture type
				std::string typeName = typeAttr.as_string();
				printf("Type: %s ", typeName.c_str());
				if (typeName == "2D")
				{
					sampler->SetType(Texture::Type::TEXTURE_2D);
				}
				else if (typeName == "cube")
				{
					sampler->SetType(Texture::Type::TEXTURE_CUBE);
				}


				// Read tiling
				Texture::Tiling tilingU = Texture::Tiling::REPEAT;
				Texture::Tiling tilingV = Texture::Tiling::REPEAT;
				Texture::Tiling tilingW = Texture::Tiling::REPEAT;

				if (tilingUAttr)
				{
					std::string tilingTypeName = tilingUAttr.as_string();
					if (tilingTypeName == "clamp")
					{
						tilingU = Texture::Tiling::CLAMP_TO_EDGE;
					}
				}

				if (tilingVAttr)
				{
					std::string tilingTypeName = tilingVAttr.as_string();
					if (tilingTypeName == "clamp")
					{
						tilingV = Texture::Tiling::CLAMP_TO_EDGE;
					}
				}
				if (tilingWAttr)
				{
					std::string tilingTypeName = tilingVAttr.as_string();
					if (tilingTypeName == "clamp")
					{
						tilingW = Texture::Tiling::CLAMP_TO_EDGE;
					}
				}


				// Read filter
				Texture::Filtering filtering = Texture::Filtering::NEAREST;
				if (filerAttr)
				{
					std::string filterTypeName = filerAttr.as_string();
					if (filterTypeName == "nearest")
					{
						filtering = Texture::Filtering::NEAREST;
					}
					else if (filterTypeName == "linear")
					{
						filtering = Texture::Filtering::LINEAR;
					}
					else if (filterTypeName == "bilinear")
					{
						filtering = Texture::Filtering::BILINEAR;
					}
					else if (filterTypeName == "trilinear")
					{
						filtering = Texture::Filtering::TRILINEAR;
					}
				}

				if (!usageAttr.empty())
				{
					String usage = usageAttr.as_string();
					if (usage == "diffuse_map")
					{
						sampler->SetUsage(Sampler::Usage::DIFFUSE_MAP);
					}
				}

				printf("\n");

				sampler->SetName(name);
				sampler->SetTiling(tilingU, tilingV, tilingW);
				sampler->SetFiltering(filtering);
				sampler->Init();

				AddSampler(sampler);

				uniformNode = uniformNode.next_sibling();
			}
#endif
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::createDecriptorPool()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			std::array<VkDescriptorPoolSize, 2> poolSizes = {};
			poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSizes[0].descriptorCount = 1;
			poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			poolSizes[1].descriptorCount = 1;

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = poolSizes.size();
			poolInfo.pPoolSizes = poolSizes.data();
			poolInfo.maxSets = 1;

			SH_ASSERT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_descriptorPool) == VK_SUCCESS,
				"failed to create descriptor pool!");
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::createDescriptorSetLayout()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			VkDescriptorSetLayoutBinding uboLayoutBinding = {};
			uboLayoutBinding.binding = 0;
			uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uboLayoutBinding.descriptorCount = 1;
			uboLayoutBinding.pImmutableSamplers = nullptr;
			uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			
			std::array<VkDescriptorSetLayoutBinding, 1> bindings = { uboLayoutBinding };

			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = bindings.size();
			layoutInfo.pBindings = bindings.data();

			SH_ASSERT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_descriptorSetLayout) == VK_SUCCESS,
				"failed to create descriptor set layout!");
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::createDescriptorSet()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			VkDescriptorSetLayout layouts[] = { m_descriptorSetLayout };
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = m_descriptorPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = layouts;

			SH_ASSERT(vkAllocateDescriptorSets(device, &allocInfo, &m_descriptorSet) == VK_SUCCESS,
				"failed to allocate descriptor set!");

			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = m_uniformBuffer;
			bufferInfo.offset = 0;
			//bufferInfo.range = sizeof(UniformBufferObject);

			std::array<VkWriteDescriptorSet, 1> descriptorWrites = {};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = m_descriptorSet;
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			SH_ASSERT(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) == VK_SUCCESS, "failed to create buffer!");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

			SH_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) == VK_SUCCESS, "failed to allocate vertex buffer memory!");

			vkBindBufferMemory(device, buffer, bufferMemory, 0);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkCommandBuffer commandBuffer = driver->beginSingleTimeCommands();

			VkBufferCopy copyRegion = {};
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

			driver->endSingleTimeCommands(commandBuffer);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		uint32_t VulkanUniformBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkPhysicalDevice physicalDevice = driver->GetVulkanPhysicalDevice();

			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
			{
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
				{
					return i;
				}
			}

			SH_ASSERT(0, "failed to find suitable memory type!");
			return 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}