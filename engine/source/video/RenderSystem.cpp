#include "video/RenderSystem.h"

namespace sh
{

namespace video
{
    RenderSystem::RenderSystem()
    {
    }

    RenderSystem::~RenderSystem()
    {
    }

    void RenderSystem::SetViewport(const math::Rect& viewport)
    {
        m_viewport = viewport;
    }

    const math::Rect& RenderSystem::GetViewport() const
    {
        return m_viewport;
    }

    void RenderSystem::SetDepthRange(float zMin, float zMax)
    {
        m_depthRange.x = zMin;
        m_depthRange.y = zMax;
    }

    const math::Vector2& RenderSystem::GetDepthRange() const
    {
        return m_depthRange;
    }

    void RenderSystem::SetClearColorValue(const math::Vector4& clearColor)
    {
        m_clearColorValue = clearColor;
    }

    void RenderSystem::SetClearDepthValue(float clearDepth)
    {
        m_clearDepthValue = clearDepth;
    }

    void RenderSystem::SetClearStencilValue(uint32_t clearStencil)
    {
        m_clearStencilValue = clearStencil;
    }

    const math::Vector4& RenderSystem::GetClearColorValue() const
    {
        return m_clearColorValue;
    }

    float RenderSystem::GetClearDepthValue() const
    {
        return m_clearDepthValue;
    }

    uint32_t RenderSystem::GetClearStencilValue() const
    {
        return m_clearStencilValue;
    }

} // video
	
} // sh
