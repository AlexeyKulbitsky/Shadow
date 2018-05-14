#include "ComponentEditor.h"
#include "../propertyeditors/PropertyEditorsFactory.h"

ComponentEditor::ComponentEditor(sh::Component* component)
{
	m_toggleButton.reset(new sh::gui::Button(component->GetTypeName()));
	m_toggleButton->SetToggleable(true);
	m_toggleButton->SetMaximumHeight(20);
	m_toggleButton->OnToggle.Connect(std::bind(&ComponentEditor::OnButtonToggled, this,
		std::placeholders::_1, std::placeholders::_2));

	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	layout->SetMargins(2, 2, 2, 2);
	layout->SetSpacing(2);
	layout->AddWidget(m_toggleButton);

	m_contentsWidget.reset(new sh::gui::Widget());
	m_contentsLayout.reset(new sh::gui::VerticalLayout());
	m_contentsLayout->SetMargins(2, 2, 2, 2);
	m_contentsLayout->SetSpacing(2);
	m_contentsWidget->SetLayout(m_contentsLayout);
	layout->AddWidget(m_contentsWidget);

	SetLayout(layout);


	auto properties = sh::ObjectFactory::GetInstance()->GetProperties(component->GetTypeName());
	if (!properties)
		return;

	for (const auto& property : (*properties))
	{
		auto propertyEditor = PropertyEditorsFactory::GetInstance()->CreatePropertyEditor(component, property.second);
		if (propertyEditor)
			m_contentsLayout->AddWidget(propertyEditor);
	}
    
    SetName(component->GetTypeName());
}

void ComponentEditor::OnButtonToggled(bool toggled, const sh::gui::ButtonPtr& sender)
{
	//SetMaximumHeight(std::numeric_limits<sh::u32>::max());
	if (!toggled)
	{
		//m_contentsWidget->SetMaximumHeight(100000);
		//m_contentsWidget->SetVisible(true);
		m_layout->AddWidget(m_contentsWidget);
	}
	else
	{
		m_layout->RemoveWidget(m_contentsWidget);
		//m_contentsWidget->SetMaximumHeight(0);
		//m_contentsWidget->SetVisible(false);
	}
	UpdateLayout();

	// 	auto rect = m_layout->GetRect();
	// 	auto height = rect.GetHeight();
	// 	height += m_layout->GetTopMargin();
	// 	height += m_layout->GetBottomMargin();
	// 	SetMaximumHeight(height);

	if (m_parent)
	{
		m_parent->UpdateLayout();
	}
}
