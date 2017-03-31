#ifndef SHADOW_RENDER_PASS_INCLUDE
#define SHADOW_RENDER_PASS_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		struct RenderPipelineDescription
		{
			BlendingStatePtr blendingState;
			RasterizationStatePtr rasterizationState;
			DepthStencilStatePtr depthStencilState;

			ShaderPtr vertexShader;
			ShaderPtr fragmentShader;
			ShaderPtr geometryShader;
		};

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
			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const = 0;
			virtual const UniformBufferPtr& GetUniformBuffer() const = 0;
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const = 0;
			
			const DepthStencilStatePtr& GetDepthStencilState() const { return m_depthStencilState; }
			const RasterizationStatePtr& GetRasterizationState() const { return m_rasterizationState; }
			const BlendingStatePtr& GetBlendingState() const { return m_blendingState; }
			
			bool HasVertexShader() const { return m_description.vertexShader != nullptr; }
			bool HasFragmentShader() const { return m_description.fragmentShader != nullptr; }
			bool HasGeometryShader() const { return m_description.geometryShader != nullptr; }

			const ShaderPtr& GetVertexShader() const { return m_description.vertexShader; }
			const ShaderPtr& GetFragmentShader() const { return m_description.fragmentShader; }
			const ShaderPtr& GetGeometryShader() const { return m_description.geometryShader; }
			
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

			RenderPipelineDescription m_description;

			Layer m_layer;
		};
	}
}

#endif
