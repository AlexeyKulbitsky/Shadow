#ifndef SH_GL_RENDER_SYSTEM_COMMON_INCLUDE
#define SH_GL_RENDER_SYSTEM_COMMON_INCLUDE

#include "video/RenderSystem.h"
#include "video/GL/GLCommon.h"
#include "video/GL/GLContext/GLContextManager.h"

namespace sh
{

namespace video
{

    class GLRenderSystemCommon : public RenderSystem
    {
    public:
        void BeginRendering() override final;
        void EndRendering() override final;

        // Viewport management
        void SetViewport(const math::Rect& viewport) override final;
        void SetDepthRange(float zMin, float zMax) override final;

        // Buffers clearing
        void SetClearColorValue(const math::Vector4& clearColor) override final;
        void SetClearDepthValue(float clearDepth) override final;
        void SetClearStencilValue(uint32_t clearStencil) override final;

        GLContextManager* GetContextManager();

    protected:
        GLContextManager* m_contextManager = nullptr;
    };

} // video
 
} // sh

#endif
