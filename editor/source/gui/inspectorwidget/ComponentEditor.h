#ifndef COMPONENT_EDITOR_INCLUDE
#define COMPONENT_EDITOR_INCLUDE

#include <Shadow.h>
class ComponentEditor : public sh::gui::Widget
{
public:
	ComponentEditor(sh::Component* component);

protected:
	void OnButtonToggled(bool toggled, const sh::gui::ButtonPtr& sender);

protected:
	sh::gui::ButtonPtr m_toggleButton;
	sh::gui::WidgetPtr m_contentsWidget;
	sh::gui::VerticalLayoutPtr m_contentsLayout;
};

#endif
