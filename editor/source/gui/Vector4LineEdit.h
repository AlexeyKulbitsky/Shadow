#ifndef SHADOW_VECTOR4_LINE_EDIT_INCLUDE
#define SHADOW_VECTOR4_LINE_EDIT_INCLUDE

#include <gui/FloatLineEdit.h>

class Vector4LineEdit : public sh::gui::Widget
{
public:
	Vector4LineEdit();

	const sh::math::Vector4& GetValue() const { return m_value; }
	void SetValue(const sh::math::Vector4& value);

	// Signals
	sh::Event<void, float> xValueChanged;
	sh::Event<void, float> yValueChanged;
	sh::Event<void, float> zValueChanged;
	sh::Event<void, float> wValueChanged;
	sh::Event<void, const sh::math::Vector4&> valueChanged;

protected:
	void OnXvalueChanged(float value);
	void OnYvalueChanged(float value);
	void OnZvalueChanged(float value);
	void OnWvalueChanged(float value);

protected:
	sh::math::Vector4 m_value = sh::math::Vector4(0.0f);

	std::array<sh::gui::FloatLineEditPtr, 4U> m_lineEdits;
};

#endif