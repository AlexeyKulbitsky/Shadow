#ifndef SHADOW_RENDER_PASS_INCLUDE
#define SHADOW_RENDER_PASS_INCLUDE

#include <pugixml.hpp>

namespace sh
{
	namespace video
	{
		class ShaderProgram;
		class UniformBuffer;
		class RenderState;

		class RenderPass
		{
		public:
			RenderPass();			
			~RenderPass();

			RenderPass* Clone();

			void Load(const pugi::xml_node &node);
			ShaderProgram* GetShaderProgram() const { return m_shaderProgram; }
			RenderState* GetRenderState() const { return m_renderState; }

		private:
			void LoadRenderState(const pugi::xml_node &node);

		protected:
			ShaderProgram* m_shaderProgram;
			RenderState* m_renderState;
		};
	}
}

#endif
