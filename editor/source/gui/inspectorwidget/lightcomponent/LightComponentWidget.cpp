#include "LightComponentWidget.h"

LightComponentWidget::LightComponentWidget()
	:ExpandableWidget("Light")
{
	m_colorPicker.reset(new ColorPicker());
}

void LightComponentWidget::SetLightComponent(sh::LightComponent* component)
{
	m_lightComponent = component;

	m_contentsLayout->Clear();

	if (!m_lightComponent)
		return;

	auto guiMgr = sh::gui::GuiManager::GetInstance();

	// Light type
	sh::gui::ComboBoxPtr lightTypeCombobox(new sh::gui::ComboBox());
	lightTypeCombobox->AddItem("Point");
	lightTypeCombobox->AddItem("Directional");
	lightTypeCombobox->AddItem("Spot");
	lightTypeCombobox->AddItem("Ambient");

	auto lightType = m_lightComponent->GetType();
	lightTypeCombobox->SetSelectedItem(static_cast<size_t>(lightType));

	sh::gui::LabelPtr lightTypeLabel(new sh::gui::Label("Type"));
	sh::gui::WidgetPtr lightTypeWidget(new sh::gui::Widget());
	sh::gui::HorizontalLayoutPtr lightTypeLayout(new sh::gui::HorizontalLayout());
	lightTypeLayout->AddWidget(lightTypeLabel);
	lightTypeLayout->AddWidget(lightTypeCombobox);
	lightTypeWidget->SetLayout(lightTypeLayout);
	lightTypeWidget->SetMaximumHeight(20);

	// Light color
	sh::gui::LabelPtr colorLabel(new sh::gui::Label("Color"));
	sh::gui::ButtonPtr pickerButton = guiMgr->GetStyle()->GetButton("PickerButton");
	
	pickerButton->OnRelease.Connect(std::bind(&LightComponentWidget::OnColorPickerButtonReleased, this));
	sh::gui::WidgetPtr colorWidget(new sh::gui::Widget());
	sh::gui::HorizontalLayoutPtr colorLayout(new sh::gui::HorizontalLayout());
	colorLayout->AddWidget(colorLabel);
	colorLayout->AddWidget(pickerButton);
	colorWidget->SetLayout(colorLayout);
	colorWidget->SetMaximumHeight(20);

	m_contentsLayout->AddWidget(lightTypeWidget);
	m_contentsLayout->AddWidget(colorWidget);
}

void LightComponentWidget::OnColorPickerButtonReleased()
{
	sh::gui::GuiManager::GetInstance()->AddChild(m_colorPicker, false);
}