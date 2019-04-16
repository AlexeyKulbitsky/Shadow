#include "video/RenderPipeline.h"
#include "Managers/RenderStateManager.h"


namespace sh
{

namespace video
{

    RenderPipeline::RenderPipeline()
    {
    }

    RenderPipeline::~RenderPipeline()
    {

    }

    RenderPipelinePtr RenderPipeline::Create(const RenderPipelineDescription& description)
    {
        RenderPipelinePtr result = RenderStateManager::GetInstance()->CreateRenderPipeline(description);

        return result;
    }

}

}