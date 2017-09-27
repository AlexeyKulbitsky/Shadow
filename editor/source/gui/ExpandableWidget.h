#ifndef SHADOW_EDITOR_EXPANDABLE_WIDGET_INCLUDE
#define SHADOW_EDITOR_EXPANDABLE_WIDGET_INCLUDE

#include <Shadow.h>

class ExpandableWidget : public sh::gui::Widget
{
public:
	ExpandableWidget(const sh::String& title);

protected:
	void OnButtonToggled(bool toggled, const sh::gui::ButtonPtr& sender);

private:
	sh::gui::ButtonPtr m_toggleButton;

};

#endif