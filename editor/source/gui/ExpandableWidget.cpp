#include "ExpandableWidget.h"

ExpandableWidget::ExpandableWidget(const sh::String& title)
{
	m_toggleButton.reset(new sh::gui::Button(title));
	m_toggleButton->SetToggleable(true);
	m_toggleButton->SetMaximumHeight(20);
	m_toggleButton->OnToggle.Connect(std::bind(&ExpandableWidget::OnButtonToggled, this,
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
}

void ExpandableWidget::OnButtonToggled(bool toggled, const sh::gui::ButtonPtr& sender)
{
	//SetMaximumHeight(std::numeric_limits<sh::u32>::max());
	if (!toggled)
	{
		m_contentsWidget->SetMaximumHeight(100000);
		m_contentsWidget->SetVisible(true);
	}
	else
	{
		m_contentsWidget->SetMaximumHeight(0);
		m_contentsWidget->SetVisible(false);
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
