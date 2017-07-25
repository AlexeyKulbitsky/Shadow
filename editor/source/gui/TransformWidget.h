#ifndef SHADOW_EDITOR_TRANSFORM_WIDGET_INCLUDE
#define SHADOW_EDITOR_TRANSFORM_WIDGET_INCLUDE

#include <Shadow.h>

#include "Vector3LineEdit.h"

class TransformWidget
{
public:
	TransformWidget();
	~TransformWidget();

	const sh::gui::WidgetPtr& GetWidget() const { return m_widget; }

	void SetTransformComponent(sh::TransformComponent* component);

	void Update();

private:
	void OnPositinChanged(const sh::math::Vector3f& position);
	void OnRotationChanged(const sh::math::Vector3f& rotation);
	void OnScaleChanged(const sh::math::Vector3f& scale);

private:
	sh::gui::WidgetPtr m_widget;
	Vector3LineEdit m_positionEdit;
	Vector3LineEdit m_rotationEdit;
	Vector3LineEdit m_scaleEdit;

	sh::TransformComponent* m_transformComponent = nullptr;
};

#endif
