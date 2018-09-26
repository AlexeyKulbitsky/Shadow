#include "SceneWindow.h"

void SceneWindow::SetRect(const sh::math::Rect& rect)
{
    Widget::SetRect(rect);
    
    auto camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();
    camera->SetProjection(3.1415926535f / 3.0f,
                            static_cast<float>( rect.GetWidth() ),
                            static_cast<float>( rect.GetHeight() ), 0.1f, 1000.0f);
    camera->SetViewport(rect);
    
    //sh::Device::GetInstance()->GetDriver()->SetViewport(rect.upperLeftCorner.x, rect.upperLeftCorner.y, rect.GetWidth(), rect.GetHeight());
}

bool SceneWindow::ProcessEvent(sh::gui::GUIEvent& ev)
{
    return Widget::ProcessEvent(ev);
    const bool isPointInside = m_rect.IsPointInside(ev.x, ev.y);
    
    switch (ev.type)
    {
        case sh::gui::EventType::PointerUp:
        {
            
        }
            break;
        default:
            break;
    }
    
    return false;
}
