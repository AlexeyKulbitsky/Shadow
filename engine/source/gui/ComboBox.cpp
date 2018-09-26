#include "ComboBox.h"

#include "GuiManager.h"
#include "../serialization/ObjectFactory.h"

namespace sh
{

namespace gui
{

	ComboBox::ComboBox()
		: ComboBox(math::Rect(0, 0, 10, 10))
	{
        m_name = "ComboBox";
	}

	ComboBox::ComboBox(const math::Rect& rect)
	{
		VerticalLayoutPtr scrollWidgetLayout(new VerticalLayout());
		math::Rect scrollRect(rect.upperLeftCorner.x, rect.lowerRightCorner.y,
			rect.lowerRightCorner.x, rect.lowerRightCorner.y + rect.GetHeight() + 200);
		m_scrollWidget.reset(new ScrollWidget());
		m_scrollWidget->SetLayout(scrollWidgetLayout);
		m_scrollWidget->SetRect(scrollRect);

		m_button.reset(new Button(rect));
		m_button->SetToggleable(true);
		m_button->OnToggle.Connect(std::bind(&ComboBox::OnButtonToggled, this, std::placeholders::_1));

		VerticalLayoutPtr layout(new VerticalLayout());
		layout->AddWidget(m_button);
		SetLayout(layout);
        m_name = "ComboBox";
	}
    
    void ComboBox::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<ComboBox>("UI");
        ObjectFactory::GetInstance()->RegisterParentProperties<ComboBox, Widget>();
    }

	void ComboBox::Render(video::Painter* painter)
	{
		if (!m_visible)
			return;

		//if (m_isInFocus)
		//	return;

		m_button->Render(painter);
		if (m_showList)
		{
			m_scrollWidget->Render(painter);
		}
	}

	void ComboBox::RenderBackground(video::Painter* painter)
	{
		m_button->RenderBackground(painter);
		if (m_showList)
		{
			m_scrollWidget->RenderBackground(painter);
		}
	}

	void ComboBox::RenderText(video::Painter* painter)
	{
		m_button->RenderText(painter);
		if (m_showList)
		{
			m_scrollWidget->RenderText(painter);
		}
	}

	bool ComboBox::ProcessEvent(GUIEvent& ev)
	{
		if (m_button->ProcessEvent(ev))
		{
			return true;
		}
		else
		{
			if (m_showList)
			{
				m_scrollWidget->ProcessEvent(ev);
				return true;
			}
			return false;
		}
	}

	void ComboBox::AddItem(const String& text)
	{
		ButtonPtr button(new Button(text));
		button->SetToggleable(true);
		button->SetHeight(20);
		button->OnToggle.Connect(std::bind(&ComboBox::OnItemToggled, this, 
			std::placeholders::_1, std::placeholders::_2));
		m_scrollWidget->GetLayout()->AddWidget(button);
		
		u32 childrenCount = m_scrollWidget->GetLayout()->GetItemsCount();

		if (childrenCount == 1U)
		{
			m_button->SetText(text);
			m_currentSelectedItem = button;
			m_currentSelectedItem->SetToggled(true);
		}

		m_scrollWidget->UpdateLayout();
	}

	void ComboBox::RemoveItem(const String& text)
	{
		const auto& scrollWidgetLayout = m_scrollWidget->GetLayout();
		u32 itemsCount = scrollWidgetLayout->GetItemsCount();
		for (u32 i = 0U; i < itemsCount; ++i)
		{
			const auto& button = std::static_pointer_cast<Button>(scrollWidgetLayout->GetWidget(i));
			if (button->GetText() == text)
			{
				scrollWidgetLayout->RemoveWidget(button);
				m_scrollWidget->UpdateLayout();
				return;
			}
		}
	}

	void ComboBox::RemoveItem(u32 index)
	{
		const auto& scrollWidgetLayout = m_scrollWidget->GetLayout();
		u32 itemsCount = scrollWidgetLayout->GetItemsCount();
		if (index >= itemsCount)
			return;

		const auto& widget = scrollWidgetLayout->GetWidget(index);
		scrollWidgetLayout->RemoveWidget(widget);
		m_scrollWidget->UpdateLayout();
	}

	void ComboBox::SetSelectedItem(const String& item)
	{

	}

	void ComboBox::SetSelectedItem(const u32 index)
	{
		const auto& scrollWidgetLayout = m_scrollWidget->GetLayout();
		if (index >= scrollWidgetLayout->GetItemsCount())
			return;

		const auto& button = std::static_pointer_cast<Button>(scrollWidgetLayout->GetWidget(index));
		button->SetToggled(true);
	}

	void ComboBox::OnButtonToggled(bool toggled)
	{
		m_showList = toggled;
		//m_isInFocus = toggled;

		if (toggled)
		{
			GuiManager::GetInstance()->SetFocusWidget(shared_from_this());
		}
		else
		{
			if (IsInFocus())
				GuiManager::GetInstance()->SetFocusWidget(nullptr);
		}
	}

	void ComboBox::OnItemToggled(bool toggled, const ButtonPtr& sender)
	{
		if (sender == m_currentSelectedItem)
		{
			if (!toggled)
				sender->SetToggled(true);
			return;
		}

		if (!toggled)
			return;

		m_currentSelectedItem = sender;

		const auto& scrollWidgetLayout = m_scrollWidget->GetLayout();
		u32 itemsCount = scrollWidgetLayout->GetItemsCount();
		for (u32 i = 0U; i < itemsCount; ++i)
		{
			const auto& button = std::static_pointer_cast<Button>(scrollWidgetLayout->GetWidget(i));
			if (button != sender && button->IsToggled())
			{
				button->SetToggled(false);
			}
			if (button == sender)
			{
				m_button->SetText(button->GetText());
				OnItemChanged(i);
			}
		}

		m_button->SetToggled(false);
	}

	void ComboBox::UpdateLayout()
	{
		Widget::UpdateLayout();

		m_scrollWidget->SetPosition(m_rect.upperLeftCorner.x, 
									m_rect.upperLeftCorner.y + m_button->GetRect().GetHeight());
		m_scrollWidget->SetWidth(m_rect.GetWidth());
	}

} // gui

} // sh
