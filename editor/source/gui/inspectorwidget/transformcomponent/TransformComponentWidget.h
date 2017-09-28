#ifndef SHADOW_EDITOR_TRANSFORM_WIDGET_INCLUDE
#define SHADOW_EDITOR_TRANSFORM_WIDGET_INCLUDE

#include "../../ExpandableWidget.h"
#include "../../Vector3LineEdit.h"
#include "../../ParamEdit.h"

class TransformComponentWidget : public ExpandableWidget
{
public:
	TransformComponentWidget();
	~TransformComponentWidget();

	void SetTransformComponent(sh::TransformComponent* component);
	void Update();

private:
	void OnPositinChanged(const sh::math::Vector3f& position);
	void OnRotationChanged(const sh::math::Vector3f& rotation);
	void OnScaleChanged(const sh::math::Vector3f& scale);

private:
	sh::gui::WidgetPtr m_widget;
	sh::SPtr<Vector3LineEdit> m_positionEdit;
	sh::SPtr<Vector3LineEdit> m_rotationEdit;
	sh::SPtr<Vector3LineEdit> m_scaleEdit;

	sh::SPtr<ParamEdit<sh::math::Vector3f>> m_testEdit;

	sh::TransformComponent* m_transformComponent = nullptr;
};

#endif
