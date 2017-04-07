#ifndef SHADOW_RENDER_PASS_INCLUDE
#define SHADOW_RENDER_PASS_INCLUDE

#include "../Globals.h"
#include "GpuParamDescription.h"

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

			GpuParamDescription paramsDescription;
			VertexInputDeclarationPtr vertexDeclaration;
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

			virtual void Load(const pugi::xml_node &node);
			virtual void Init() {}
			virtual void Unload();

			const String& GetName() const { return m_name; }

			virtual const ShaderProgramPtr& GetShaderProgram() const = 0;
			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const = 0;
			virtual const UniformBufferPtr& GetUniformBuffer() const = 0;
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const = 0;
			
			const DepthStencilStatePtr& GetDepthStencilState() const { return m_description.depthStencilState; }
			const RasterizationStatePtr& GetRasterizationState() const { return m_description.rasterizationState; }
			const BlendingStatePtr& GetBlendingState() const { return m_description.blendingState; }
			
			bool HasVertexShader() const { return m_description.vertexShader != nullptr; }
			bool HasFragmentShader() const { return m_description.fragmentShader != nullptr; }
			bool HasGeometryShader() const { return m_description.geometryShader != nullptr; }

			const ShaderPtr& GetVertexShader() const { return m_description.vertexShader; }
			const ShaderPtr& GetFragmentShader() const { return m_description.fragmentShader; }
			const ShaderPtr& GetGeometryShader() const { return m_description.geometryShader; }
			
			Layer GetLayer() const { return m_layer; }

			const GpuParamDescription& GetGpuParamsDescription() const { return m_description.paramsDescription; }

			static RenderPipelinePtr Create(const RenderPipelineDescription& description);

		protected:
			void LoadStates(const pugi::xml_node& node);

		protected:
			String m_name;

			RenderPipelineDescription m_description;
			GpuParamDescription m_paramsDesc;

			Layer m_layer;
		};
	}
}

#endif
