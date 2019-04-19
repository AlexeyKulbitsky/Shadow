#ifndef SHADOW_MATERIAL_INCLUDE
#define SHADOW_MATERIAL_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"
#include "video/RenderPipeline.h"

namespace sh
{

namespace video
{

    class SHADOW_API Material
    {
    public:
        Material();
        void SetRenderPipeline(const RenderPipelinePtr& renderPipeline);

        const RenderPipelinePtr& GetRenderPipeline();
        const std::string& GetName() const;

        const GpuParamsPtr& GetGpuParams() const;
        const MaterialParamsPtr& GetMaterialParams() const;

    private:
        std::string m_name = "default";
        RenderPipelinePtr m_renderPipeline;

        GpuParamsPtr m_gpuParams;
        MaterialParamsPtr m_materialParams;
    };

} // video

} // sh

#endif