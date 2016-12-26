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
			enum class Layer
			{
				BACKGROUND = 0,
				MAIN,

				COUNT
			};
			RenderPass();			
			~RenderPass();

			RenderPass* Clone();

			void Load(const pugi::xml_node &node);

			const String& GetName() const { return m_name; }
			ShaderProgram* GetShaderProgram() const { return m_shaderProgram; }
			VertexInputDeclaration* GetVertexInputDeclaration() const { return m_vertexInputDeclaration; }
			UniformBuffer* GetUniformBuffer() const { return m_uniformBuffer; }
			RenderState* GetRenderState() const { return m_renderState; }
			Layer GetLayer() const { return m_layer; }

		protected:
			String m_name;
			ShaderProgram* m_shaderProgram;
			VertexInputDeclaration* m_vertexInputDeclaration;
			UniformBuffer* m_uniformBuffer;
			RenderState* m_renderState;
			Layer m_layer;
		};
	}
}

#endif
