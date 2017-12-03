#include "LightComponentWidget.h"

#include "../../Vector3LineEdit.h"

LightComponentWidget::LightComponentWidget()
	:ExpandableWidget("Light")
{
	m_colorPicker.reset(new ColorPicker());
	m_colorPicker->colorChanged.Connect(std::bind(&LightComponentWidget::OnColorChanged, this, std::placeholders::_1));

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("white");
	m_colorWidget = icon->Clone();
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

	m_colorWidget->SetColor(sh::math::Vector4(m_lightComponent->GetColor(), 1.0f));
	sh::gui::ButtonPtr pickerButton = guiMgr->GetStyle()->GetButton("PickerButton");
	pickerButton->SetMaximumWidth(20);
	
	pickerButton->OnRelease.Connect(std::bind(&LightComponentWidget::OnColorPickerButtonReleased, this));
	sh::gui::WidgetPtr colorWidget(new sh::gui::Widget());
	sh::gui::HorizontalLayoutPtr colorLayout(new sh::gui::HorizontalLayout());
	colorLayout->AddWidget(colorLabel);
	colorLayout->AddWidget(m_colorWidget);
	colorLayout->AddWidget(pickerButton);
	colorWidget->SetLayout(colorLayout);
	colorWidget->SetMaximumHeight(20);

	m_contentsLayout->AddWidget(lightTypeWidget);
	m_contentsLayout->AddWidget(colorWidget);

	switch (lightType)
	{
		case sh::scene::Light::Type::DIRECTIONAL:
		{
			sh::gui::WidgetPtr directionWidget(new sh::gui::Widget());
			sh::gui::HorizontalLayoutPtr directionLayout(new sh::gui::HorizontalLayout());

			sh::gui::LabelPtr label(new sh::gui::Label("Direction"));
			sh::SPtr<Vector3LineEdit> editWidget(new Vector3LineEdit());
			editWidget->valueChanged.Connect(std::bind(&LightComponentWidget::OnDirectionChanged, this,
				std::placeholders::_1));
			editWidget->SetValue(m_lightComponent->GetDirection());

			directionLayout->AddWidget(label);
			directionLayout->AddWidget(editWidget);
			directionWidget->SetLayout(directionLayout);
			directionWidget->SetMaximumHeight(20);

			m_contentsLayout->AddWidget(directionWidget);
		}
			break;
		case sh::scene::Light::Type::POINT:
			break;
		case sh::scene::Light::Type::AMBIENT:
			break;
		case sh::scene::Light::Type::SPOT:
			break;
		default:
			break;
	}
}

void LightComponentWidget::OnColorPickerButtonReleased()
{
	m_colorPicker->SetColor(sh::math::Vector4(m_lightComponent->GetColor(), 1.0f));
	sh::gui::GuiManager::GetInstance()->AddChild(m_colorPicker, false);
}

void LightComponentWidget::OnColorChanged(const sh::math::Vector4& color)
{
	m_colorWidget->SetColor(sh::math::Vector4(color));
	m_lightComponent->SetColor(sh::math::Vector3(color.x, color.y, color.z));
}

void LightComponentWidget::OnDirectionChanged(const sh::math::Vector3& direction)
{
	auto dir = direction.GetNormalized();
	m_lightComponent->SetDirection(dir);
}