#include "video/GL/GLRenderSystemCommon.h"
#include "video/GL/GLDebug.h"
#include "video/GL/Managers/GLHardwareBufferManager.h"

namespace sh
{

namespace video
{
    GLRenderSystemCommon::GLRenderSystemCommon()
    {
        HardwareBufferManager::CreateInstance<GLHardwareBufferManager>();
    }

    GLRenderSystemCommon::~GLRenderSystemCommon()
    {
        HardwareBufferManager::DestroyInstance();
    }

    void GLRenderSystemCommon::BeginRendering()
    {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void GLRenderSystemCommon::EndRendering()
    {
        GL_CALL(glFlush());
        m_contextManager->SwapBuffers();
    }

    void GLRenderSystemCommon::SetViewport(const math::Rect& viewport)
    {
        RenderSystem::SetViewport(viewport);
        GL_CALL(glViewport(viewport.upperLeftCorner.x, viewport.upperLeftCorner.y, viewport.GetWidth(), viewport.GetHeight()));
    }

    void GLRenderSystemCommon::SetDepthRange(float zMin, float zMax)
    {
        RenderSystem::SetDepthRange(zMin, zMax);
        GL_CALL(glDepthRangef(zMin, zMax));
    }

    void GLRenderSystemCommon::SetClearColorValue(const math::Vector4& clearColor)
    {
        RenderSystem::SetClearColorValue(clearColor);
        GL_CALL(glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w));
    }

    void GLRenderSystemCommon::SetClearDepthValue(float clearDepth)
    {
        RenderSystem::SetClearDepthValue(clearDepth);
        GL_CALL(glClearDepthf(clearDepth));
    }

    void GLRenderSystemCommon::SetClearStencilValue(uint32_t clearStencil)
    {
        RenderSystem::SetClearStencilValue(clearStencil);
        GL_CALL(glClearStencil(static_cast<GLint>(clearStencil)));
    }

    GLContextManager* GLRenderSystemCommon::GetContextManager()
    {
        return m_contextManager;
    }

} // video

} // sh