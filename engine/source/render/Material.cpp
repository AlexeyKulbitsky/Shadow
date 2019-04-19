#include "render/Material.h"
#include "render/MaterialParams.h"
#include "video/GpuPipelineParamsInfo.h"
#include "video/GpuParams.h"

namespace sh
{

namespace video
{
    Material::Material() 
    {
    }

    void Material::SetRenderPipeline(const RenderPipelinePtr& renderPipeline)
    {
        m_renderPipeline = renderPipeline;

        m_gpuParams = GpuParams::Create(m_renderPipeline->GetParamsInfo());
        m_materialParams.reset(new MaterialParams(m_gpuParams));

    }

    const RenderPipelinePtr& Material::GetRenderPipeline() 
    { 
        return m_renderPipeline; 
    }

    const std::string& Material::GetName() const 
    { 
        return m_name; 
    }

    const GpuParamsPtr& Material::GetGpuParams() const 
    { 
        return m_gpuParams; 
    }

    const MaterialParamsPtr& Material::GetMaterialParams() const 
    { 
        return m_materialParams; 
    }

} // video

} // sh