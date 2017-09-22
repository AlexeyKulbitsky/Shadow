#ifndef SHADOW_VECTOR2_LINE_EDIT_INCLUDE
#define SHADOW_VECTOR2_LINE_EDIT_INCLUDE

#include <gui/FloatLineEdit.h>

class Vector2LineEdit : public sh::gui::Widget
{
public:
	Vector2LineEdit();

	const sh::math::Vector2f& GetValue() const { return m_value; }
	void SetValue(const sh::math::Vector2f& value);

	// Signals
	sh::Event<void, float> xValueChanged;
	sh::Event<void, float> yValueChanged;
	sh::Event<void, const sh::math::Vector2f&> valueChanged;

protected:
	void OnXvalueChanged(float value);
	void OnYvalueChanged(float value);

protected:
	sh::math::Vector2f m_value = sh::math::Vector2f(0.0f);

	std::array<sh::gui::FloatLineEditPtr, 2U> m_lineEdits;
};

#endif