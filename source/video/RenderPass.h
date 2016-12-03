#ifndef SHADOW_RENDER_PASS_INCLUDE
#define SHADOW_RENDER_PASS_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class ShaderProgram;
		class UniformBuffer;
		class RenderState;
		class VertexInputDeclaration;

		class RenderPass
		{
		public:
			RenderPass();			
			~RenderPass();

			RenderPass* Clone();

			void Load(const pugi::xml_node &node);

			ShaderProgram* GetShaderProgram() const { return m_shaderProgram; }
			VertexInputDeclaration* GetVertexInputDeclaration() const { return m_vertexInputDeclaration; }
			UniformBuffer* GetUniformBuffer() const { return m_uniformBuffer; }
			RenderState* GetRenderState() const { return m_renderState; }

		private:
			void LoadRenderState(const pugi::xml_node &node);
			void LoadUniforms(const pugi::xml_node &node);
			void LoadAttributes(const pugi::xml_node &node);
			void LoadSamplers(const pugi::xml_node &node);

		protected:
			ShaderProgram* m_shaderProgram;
			VertexInputDeclaration* m_vertexInputDeclaration;
			UniformBuffer* m_uniformBuffer;
			RenderState* m_renderState;
		};
	}
}

#endif
