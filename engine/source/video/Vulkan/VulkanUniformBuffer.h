#ifndef SHADOW_VULKAN_UNIFORM_BUFFER_INCLUDE
#define SHADOW_VULKAN_UNIFORM_BUFFER_INCLUDE

#include "../UniformBuffer.h"

namespace sh
{
	namespace video
	{
		class VulkanUniformBuffer : public UniformBuffer
		{
		public:
			VulkanUniformBuffer() {}
			virtual ~VulkanUniformBuffer() {}

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Init() override;
			virtual void SetShaderProgram(ShaderProgram* shaderProgram) override;

		private:
			void LoadUniforms(const pugi::xml_node &node);
			void LoadSamplers(const pugi::xml_node &node);
		};
	}
}

#endif
