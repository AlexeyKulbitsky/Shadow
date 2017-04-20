#ifndef SHADOW_RENDER_PASS_INCLUDE
#define SHADOW_RENDER_PASS_INCLUDE

#include "../Globals.h"
#include "GpuParamsDescription.h"
#include "GpuPipelineParamsInfo.h"

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
			ShaderPtr tesselationControlShader;
			ShaderPtr tesselationEvaluationShader;
			ShaderPtr computeShader;

			VertexInputDeclarationPtr vertexDeclaration;
		};

		class RenderPipeline
		{
			friend class GpuParams;
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
			virtual void Init(const VertexInputDeclarationPtr&) {}
			virtual void Unload();

			const String& GetName() const { return m_name; }

			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const = 0;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			const DepthStencilStatePtr& GetDepthStencilState() const { return m_description.depthStencilState; }
			const RasterizationStatePtr& GetRasterizationState() const { return m_description.rasterizationState; }
			const BlendingStatePtr& GetBlendingState() const { return m_description.blendingState; }
			
			bool HasVertexShader() const { return m_description.vertexShader != nullptr; }
			bool HasFragmentShader() const { return m_description.fragmentShader != nullptr; }
			bool HasGeometryShader() const { return m_description.geometryShader != nullptr; }

			const ShaderPtr& GetVertexShader() const { return m_description.vertexShader; }
			const ShaderPtr& GetFragmentShader() const { return m_description.fragmentShader; }
			const ShaderPtr& GetGeometryShader() const { return m_description.geometryShader; }
			
			//const GpuPipelineParamsDescription& GetParamsDescription() const { return m_paramsDescription; }
			//const GpuPipelineParamsDescription& GetAutoParamsDescription() const { return m_autoParamsDescription; }

			const GpuPipelineParamsInfoPtr& GetParamsInfo() const { return m_paramsInfo; }
			const GpuPipelineParamsInfoPtr& GetAutoParamsInfo() const { return m_autoParamsInfo; }

			Layer GetLayer() const { return m_layer; }

			static RenderPipelinePtr Create(const RenderPipelineDescription& description);

		protected:
			void LoadStates(const pugi::xml_node& node);

		protected:
			String m_name;

			RenderPipelineDescription m_description;
			//GpuPipelineParamsDescription m_paramsDescription;
			//GpuPipelineParamsDescription m_autoParamsDescription;

			GpuPipelineParamsInfoPtr m_paramsInfo;
			GpuPipelineParamsInfoPtr m_autoParamsInfo;

			Layer m_layer;
		};
	}
}

#endif
