#ifndef SHADOW_RENDER_PIPELINE_INCLUDE
#define SHADOW_RENDER_PIPELINE_INCLUDE

#include "../Globals.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "BlendingState.h"

namespace sh
{
	namespace video
	{
		class ShaderProgram;
		class UniformBuffer;
		class VertexInputDeclaration;

		class RenderPipeline
		{
		public:
			ShaderProgram* GetShaderProgram() const { return m_shaderProgram; }
			VertexInputDeclaration* GetVertexInputDeclaration() const { return m_vertexInputDeclaration; }
			UniformBuffer* GetUniformBuffer() const { return m_uniformBuffer; }
			DepthStencilState* GetDepthStencilState() const { return m_depthStencilState; }
			RasterizationState* GetRasterizationState() const { return m_rasterizationState; }
			BlendingState* GetBlendingState() const { return m_blendingState; }

		protected:
			ShaderProgram* m_shaderProgram;
			VertexInputDeclaration* m_vertexInputDeclaration;
			UniformBuffer* m_uniformBuffer;
			DepthStencilState* m_depthStencilState;
			RasterizationState* m_rasterizationState;
			BlendingState* m_blendingState;
		};
	}
}

#endif