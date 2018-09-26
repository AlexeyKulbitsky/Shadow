#include "ColorPropertyEditor.h"

ColorPropertyEditor::ColorPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create Vector4Property editor for empty object");
	SH_ASSERT(objectProperty, "Can not create Vector4Property editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_VECTOR_4_FLOAT, "Can not create Vector4Property editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);
	SetLayout(mainLayout);
	SetMaximumHeight(20);

	m_colorPicker.reset(new ColorPicker());
	m_colorPicker->colorChanged.Connect(std::bind(&ColorPropertyEditor::OnColorChanged, this, std::placeholders::_1));

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("white");
	m_colorWidget = icon->Clone();

	auto value = objectProperty->GetValue(object);
	const auto& color = value.GetColor();

	m_colorWidget->SetColor(sh::math::Vector4(color.red, color.green, color.blue, 1.0f));
    auto pickerButtonTemplate = sh::gui::GuiManager::GetInstance()->GetStyle()->GetButton("PickerButton");
    sh::gui::ButtonPtr pickerButton = pickerButtonTemplate->Clone();
	pickerButton->SetMaximumWidth(20);

	pickerButton->OnRelease.Connect(std::bind(&ColorPropertyEditor::OnColorPickerButtonReleased, this));
	sh::gui::WidgetPtr colorWidget(new sh::gui::Widget());
	mainLayout->AddWidget(m_colorWidget);
	mainLayout->AddWidget(pickerButton);
}

void ColorPropertyEditor::OnColorChanged(const sh::video::Color& color)
{
	m_colorWidget->SetColor(sh::math::Vector4(color.red, color.green, color.blue, color.alpha));
	m_objectProperty->SetValue(m_object, color);
}

void ColorPropertyEditor::OnColorPickerButtonReleased()
{
	auto value = m_objectProperty->GetValue(m_object);
	const auto& color = value.GetColor();
	m_colorPicker->SetColor(color);
	sh::gui::GuiManager::GetInstance()->AddChild(m_colorPicker, false);
}
