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
			virtual ~RenderPipeline();

			virtual RenderPipelinePtr Clone() = 0;

			virtual void Load(const pugi::xml_node &node);
			virtual void Unload();

			const String& GetName() const { return m_name; }

			virtual const ShaderProgramPtr& GetShaderProgram() const = 0;
			virtual VertexInputDeclaration* GetVertexInputDeclaration() const = 0;
			virtual const UniformBufferPtr& GetUniformBuffer() const = 0;
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const = 0;
			/*
			virtual const ShaderProgramPtr& GetShaderProgram() const { return m_shaderProgram; }
			virtual VertexInputDeclaration* GetVertexInputDeclaration() const { return m_vertexInputDeclaration; }
			virtual const UniformBufferPtr& GetUniformBuffer() const { return m_uniformBuffer; }
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const { return m_transformUniformBuffer; }
			*/
			const DepthStencilStatePtr& GetDepthStencilState() const { return m_depthStencilState; }
			const RasterizationStatePtr& GetRasterizationState() const { return m_rasterizationState; }
			const BlendingStatePtr& GetBlendingState() const { return m_blendingState; }
			Layer GetLayer() const { return m_layer; }

		protected:
			void LoadStates(const pugi::xml_node& node);

		protected:
			String m_name;
			/*
			ShaderProgramPtr m_shaderProgram;
			VertexInputDeclaration* m_vertexInputDeclaration;
			UniformBufferPtr m_uniformBuffer;
			UniformBufferPtr m_transformUniformBuffer;
			*/

			DepthStencilStatePtr m_depthStencilState;
			RasterizationStatePtr m_rasterizationState;
			BlendingStatePtr m_blendingState;

			Layer m_layer;
		};
	}
}

#endif
