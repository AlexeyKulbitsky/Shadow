#ifndef SHADOW_RENDER_PASS_INCLUDE
#define SHADOW_RENDER_PASS_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{

    struct SHADOW_API RenderPipelineDescription
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
    };

    class SHADOW_API RenderPipeline
    {
        friend class GpuParams;
    public:
        RenderPipeline();
        virtual ~RenderPipeline();

        virtual void Init(const VertexInputDeclarationPtr&) {}

        virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const = 0;

        const DepthStencilStatePtr& GetDepthStencilState() const { return m_description.depthStencilState; }
        const RasterizationStatePtr& GetRasterizationState() const { return m_description.rasterizationState; }
        const BlendingStatePtr& GetBlendingState() const { return m_description.blendingState; }

        bool HasVertexShader() const { return m_description.vertexShader != nullptr; }
        bool HasFragmentShader() const { return m_description.fragmentShader != nullptr; }
        bool HasGeometryShader() const { return m_description.geometryShader != nullptr; }

        const ShaderPtr& GetVertexShader() const { return m_description.vertexShader; }
        const ShaderPtr& GetFragmentShader() const { return m_description.fragmentShader; }
        const ShaderPtr& GetGeometryShader() const { return m_description.geometryShader; }

        const GpuPipelineParamsInfoPtr& GetParamsInfo() const { return m_paramsInfo; }

        static RenderPipelinePtr Create(const RenderPipelineDescription& description);

    protected:
        RenderPipelineDescription m_description;
        VertexInputDeclarationPtr m_vertexDeclaration;

        GpuPipelineParamsInfoPtr m_paramsInfo;
    };

} // video

} // sh

#endif
