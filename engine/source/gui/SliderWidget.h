#ifndef SHADOW_SLIDER_WIDGET_INCLUDE
#define SHADOW_SLIDER_WIDGET_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{
	class Button;

	class SliderWidget : public Widget
	{
	public:
		SliderWidget();
		~SliderWidget();

	private:
		SPtr<Button> m_button;
	};

} // gui

} // sh

#endif
