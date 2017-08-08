#ifndef SHADOW_VECTOR3_LINE_EDIT_INCLUDE
#define SHADOW_VECTOR3_LINE_EDIT_INCLUDE

#include <gui/FloatLineEdit.h>

class Vector3LineEdit : public sh::gui::Widget
{
public:
	Vector3LineEdit();

	const sh::gui::WidgetPtr& GetWidget() const { return m_widget; }

	const sh::math::Vector3f& GetValue() const { return m_value; }
	void SetValue(const sh::math::Vector3f& value);

	// Signals
	sh::Event<void, float> xValueChanged;
	sh::Event<void, float> yValueChanged;
	sh::Event<void, float> zValueChanged;
	sh::Event<void, const sh::math::Vector3f&> valueChanged;

private:
	void OnXvalueChanged(float value);
	void OnYvalueChanged(float value);
	void OnZvalueChanged(float value);

private:
	sh::math::Vector3f m_value = sh::math::Vector3f(0.0f);

	sh::gui::WidgetPtr m_widget;
	std::array<sh::gui::FloatLineEditPtr, 3U> m_lineEdits;
};

#endif