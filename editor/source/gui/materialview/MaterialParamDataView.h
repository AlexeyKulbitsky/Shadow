#ifndef SHADOW_EDITOR_MATERIAL_PARAM_DATA_VIEW_INCLUDE
#define SHADOW_EDITOR_MATERIAL_PARAM_DATA_VIEW_INCLUDE

#include <Shadow.h>

template <typename T>
class ParamView : public sh::gui::Widget
{
public:
	ParamView() { CreateConnetions(); }

	void CreateConnetions() {}
	sh::Event<void, const T&> valueChanged;

private:
	T m_value;
};

template <>
void ParamView<sh::math::Vector3f>::CreateConnetions()
{
	sh::gui::FloatLineEditPtr xEdit(new sh::gui::FloatLineEdit());
	auto onXValueChanged = [this](float value) { m_value.x = value; };
	xEdit->OnValueChanged.Connect(std::bind(&Vector3LineEdit::OnXvalueChanged, this, std::placeholders::_1));
}

#endif