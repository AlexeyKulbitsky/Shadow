#ifndef SHADOW_EDITOR_EXPANDABLE_WIDGET_INCLUDE
#define SHADOW_EDITOR_EXPANDABLE_WIDGET_INCLUDE

#include <Shadow.h>

class ExpandableWidget : public sh::gui::Widget
{
public:
	ExpandableWidget(const sh::String& title);

protected:
	void OnButtonToggled(bool toggled, const sh::gui::ButtonPtr& sender);

protected:
	sh::gui::ButtonPtr m_toggleButton;
	sh::gui::WidgetPtr m_contentsWidget;
	sh::gui::VerticalLayoutPtr m_contentsLayout;
};

#endif