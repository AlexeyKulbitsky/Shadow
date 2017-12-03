#ifndef SHADOW_VECTOR3_LINE_EDIT_INCLUDE
#define SHADOW_VECTOR3_LINE_EDIT_INCLUDE

#include <gui/FloatLineEdit.h>

class Vector3LineEdit : public sh::gui::Widget
{
public:
	Vector3LineEdit();

	const sh::math::Vector3& GetValue() const { return m_value; }
	void SetValue(const sh::math::Vector3& value);

	// Signals
	sh::Event<void, float> xValueChanged;
	sh::Event<void, float> yValueChanged;
	sh::Event<void, float> zValueChanged;
	sh::Event<void, const sh::math::Vector3&> valueChanged;

protected:
	void OnXvalueChanged(float value);
	void OnYvalueChanged(float value);
	void OnZvalueChanged(float value);

protected:
	sh::math::Vector3 m_value = sh::math::Vector3(0.0f);

	std::array<sh::gui::FloatLineEditPtr, 3U> m_lineEdits;
};

#endif