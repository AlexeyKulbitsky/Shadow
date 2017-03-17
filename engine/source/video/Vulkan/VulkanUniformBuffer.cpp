#include "VulkanUniformBuffer.h"
#include "../../Device.h"
#include "VulkanDriver.h"

namespace sh
{
	namespace video
	{
		VulkanUniformBuffer::VulkanUniformBuffer()
		{
			

			VkDeviceSize bufferSize = sizeof(sh::math::Matrix4f);//sizeof(UniformBufferObject);

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
			UniformBuffer::Init();

			createDescriptorSetLayout();
			createDecriptorPool();
			createDescriptorSet();

		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::Upload()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			void* data;
			vkMapMemory(device, m_uniformStagingBufferMemory, 0, m_autoUniformsBatch->GetSize(), 0, &data);

			sh::math::Matrix4f mat = m_autoUniformsBatch->m_uniforms[0]->Get<sh::math::Matrix4f>();

			u32 pointer = 0U;
			for (size_t i = 0, sz = m_autoUniformsBatch->m_uniforms.size(); i < sz; ++i)
			{
				Uniform* uniform = m_autoUniformsBatch->m_uniforms[i];
				data = static_cast<char*>(data) + pointer;
				memcpy(data, uniform->GetData(), uniform->GetSize());
			}
			
			vkUnmapMemory(device, m_uniformStagingBufferMemory);

			copyBuffer(m_uniformStagingBuffer, m_uniformBuffer, m_autoUniformsBatch->GetSize());
		}

		/////////////////////////////////////////////////////////////////////////

		UniformBuffer* VulkanUniformBuffer::Clone()
		{
			VulkanUniformBuffer* buffer = new VulkanUniformBuffer();
			buffer->m_autoUniformsBatch = m_autoUniformsBatch->Clone();


			buffer->m_descriptorPool = m_descriptorPool;
			buffer->m_descriptorSet = m_descriptorSet;
			buffer->m_descriptorSetLayout = m_descriptorSetLayout;
			buffer->m_pipelineLayout = m_pipelineLayout;

			buffer->m_uniformStagingBuffer = m_uniformStagingBuffer;
			buffer->m_uniformStagingBufferMemory = m_uniformStagingBufferMemory;
			buffer->m_uniformBuffer = m_uniformBuffer;
			buffer->m_uniformBufferMemory = m_uniformBufferMemory;

			UniformBuffer* result = buffer;
			return result;
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::SetShaderProgram(ShaderProgram* shaderProgram)
		{
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanUniformBuffer::LoadUniforms(const pugi::xml_node &node)
		{
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
					
					if (valAttr)
					{
						float value = valAttr.as_float();
						uniform = new Uniform(value);
						printf("Value: %f", value);
					}

				}
				else if (typeName == "int")
				{
					if (valAttr)
					{
						int value = valAttr.as_int();
						uniform = new Uniform(value);
						printf("Value: %d", value);
					}
				}
				else if (typeName == "vec2")
				{
					SH_ASSERT(0, "Unsupported uniform type");
					if (valAttr)
					{

					}
				}
				else if (typeName == "vec3")
				{
					SH_ASSERT(0, "Unsupported uniform type");
					if (valAttr)
					{
					}
				}
				else if (typeName == "vec4")
				{
					SH_ASSERT(0, "Unsupported uniform type");
					if (valAttr)
					{

					}
				}
				else if (typeName == "mat4")
				{
					uniform = new Uniform(sh::math::Matrix4f::Identity());
					if (valAttr)
					{
						String value = valAttr.as_string();
						if (value == "model.worldMatrix")
						{						
							uniform->SetGlobalUniformName(GlobalUniformName::MODEL_WORLD_MATRIX);
						}
						else if (value == "model.worldViewProjectionMatrix")
						{
							uniform->SetGlobalUniformName(GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX);
						}
						else if (value == "camera.viewProjectionMatrix")
						{
							uniform->SetGlobalUniformName(GlobalUniformName::CAMERA_VIEW_PROJECTION_MATRIX);
						}
						else if (value == "camera.viewRotationProjectionMatrix")
						{
							uniform->SetGlobalUniformName(GlobalUniformName::CAMERA_VIEW_ROTATION_PROJECTION_MATRIX);
						}
					}
				}
				else if (typeName == "vec3Array")
				{				
					SH_ASSERT(0, "Unsupported uniform type");
					String value = valAttr.as_string();
					if (value == "light.directional.direction")
					{
						//element.globalName = GlobalUniformName::LIGHT_DIRECTIONAL_DIRECTION;
					}
				}

				printf("\n");

				uniform->SetName(name);
				AddUniform(uniform);

				uniformNode = uniformNode.next_sibling();
			}
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

			std::array<VkDescriptorPoolSize, 1> poolSizes = {};
			poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSizes[0].descriptorCount = 1;
			//poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			//poolSizes[1].descriptorCount = 1;

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = poolSizes.size();
			poolInfo.pPoolSizes = poolSizes.data();
			poolInfo.maxSets = 1;

			VkResult res = vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_descriptorPool);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create descriptor pool!");
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

			VkResult res = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_descriptorSetLayout);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create descriptor set layout!");
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
			bufferInfo.range = m_autoUniformsBatch->GetSize();

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