#ifndef SHADOW_RENDER_PASS_INCLUDE
#define SHADOW_RENDER_PASS_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderPipeline
		{
		public:
			enum class Layer
			{
				BACKGROUND = 0,
				MAIN,

				COUNT
			};
			RenderPipeline();			
			~RenderPipeline();

			RenderPipeline* Clone();

			void Load(const pugi::xml_node &node);
			void Unload();

			const String& GetName() const { return m_name; }
			ShaderProgram* GetShaderProgram() const { return m_shaderProgram.get(); }
			VertexInputDeclaration* GetVertexInputDeclaration() const { return m_vertexInputDeclaration; }
			UniformBuffer* GetUniformBuffer() const { return m_uniformBuffer.get(); }
			DepthStencilState* GetDepthStencilState() const { return m_depthStencilState.get(); }
			RasterizationState* GetRasterizationState() const { return m_rasterizationState.get(); }
			BlendingState* GetBlendingState() const { return m_blendingState.get(); }
			Layer GetLayer() const { return m_layer; }

		protected:
			String m_name;
			ShaderProgramPtr m_shaderProgram;
			VertexInputDeclaration* m_vertexInputDeclaration;
			UniformBufferPtr m_uniformBuffer;

			DepthStencilStatePtr m_depthStencilState;
			RasterizationStatePtr m_rasterizationState;
			BlendingStatePtr m_blendingState;

			Layer m_layer;
		};
	}
}

#endif
