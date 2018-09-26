#ifndef SCENE_WINDOW_INCLUDE
#define SCENE_WINDOW_INCLUDE

#include <Shadow.h>

class SceneWindow : public sh::gui::Widget
{
public:
    virtual void SetRect(const sh::math::Rect& rect) override;
    virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
};

#endif
