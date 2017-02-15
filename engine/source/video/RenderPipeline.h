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
			const ShaderProgramPtr& GetShaderProgram() const { return m_shaderProgram; }
			VertexInputDeclaration* GetVertexInputDeclaration() const { return m_vertexInputDeclaration; }
			const UniformBufferPtr& GetUniformBuffer() const { return m_uniformBuffer; }
			const DepthStencilStatePtr& GetDepthStencilState() const { return m_depthStencilState; }
			const RasterizationStatePtr& GetRasterizationState() const { return m_rasterizationState; }
			const BlendingStatePtr& GetBlendingState() const { return m_blendingState; }
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
