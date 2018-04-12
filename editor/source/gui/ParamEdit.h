#ifndef SHADOW_EDITOR_PARAM_EDIT_INCLUDE
#define SHADOW_EDITOR_PARAM_EDIT_INCLUDE

#include <Shadow.h>

template <typename T>
class ParamEdit : public sh::gui::Widget
{
public:
	ParamEdit() { CreateConnetions(); }

	void CreateConnetions() {}
	const T& GetValue() const { return m_value; }
	void SetValue(const T& value) { m_value = value; }

	sh::Event<void, const T&> valueChanged;

private:
	T m_value;
};

/*template <>
void ParamEdit<sh::math::Vector3>::CreateConnetions()
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	layout->SetSpacing(2);

	sh::gui::FloatLineEditPtr xEdit(new sh::gui::FloatLineEdit());
	auto onXValueChanged = [this](float value) { m_value.x = value; valueChanged(m_value); };
	xEdit->OnValueChanged.Connect(onXValueChanged);
	layout->AddWidget(xEdit);

	sh::gui::FloatLineEditPtr yEdit(new sh::gui::FloatLineEdit());
	auto onYValueChanged = [this](float value) { m_value.y = value; valueChanged(m_value); };
	yEdit->OnValueChanged.Connect(onYValueChanged);
	layout->AddWidget(yEdit);

	sh::gui::FloatLineEditPtr zEdit(new sh::gui::FloatLineEdit());
	auto onZValueChanged = [this](float value) { m_value.z = value; valueChanged(m_value); };
	zEdit->OnValueChanged.Connect(onZValueChanged);
	layout->AddWidget(zEdit);

	SetLayout(layout);
}*/

#endif
