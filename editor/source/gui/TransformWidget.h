#ifndef SHADOW_EDITOR_TRANSFORM_WIDGET_INCLUDE
#define SHADOW_EDITOR_TRANSFORM_WIDGET_INCLUDE

#include <Shadow.h>

class TransformWidget
{
public:
	TransformWidget();
	~TransformWidget();

	const sh::gui::WidgetPtr& GetWidget() const { return m_widget; }

private:
	sh::gui::WidgetPtr m_widget;
};

#endif
