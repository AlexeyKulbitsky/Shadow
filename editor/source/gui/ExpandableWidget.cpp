#include "ExpandableWidget.h"

ExpandableWidget::ExpandableWidget(const sh::String& title)
{
	m_toggleButton.reset(new sh::gui::Button(title));
	m_toggleButton->SetToggleable(true);
	m_toggleButton->OnToggle.Connect(std::bind(&ExpandableWidget::OnButtonToggled, this,
		std::placeholders::_1, std::placeholders::_2));

	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	layout->SetMargins(2, 2, 2, 2);
	layout->SetSpacing(2);
	layout->AddWidget(m_toggleButton);
	SetLayout(layout);
}

void ExpandableWidget::OnButtonToggled(bool toggled, const sh::gui::ButtonPtr& sender)
{
	if (toggled)
	{
		SetMaximumHeight(100000);
		SetVisible(true);
	}
	else
	{
		SetMaximumHeight(0);
		SetVisible(false);
	}
}
