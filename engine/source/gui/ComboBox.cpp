#include "ComboBox.h"

#include "GuiManager.h"

namespace sh
{

namespace gui
{

	ComboBox::ComboBox()
		: ComboBox(math::Recti(0, 0, 10, 10))
	{
	}

	ComboBox::ComboBox(const math::Recti& rect)
	{
		m_button.reset(new Button(rect));
		m_button->SetToggleable(true);
		m_button->OnToggle.Connect(std::bind(&ComboBox::OnButtonToggled, this, std::placeholders::_1));

		m_layout.reset(new VerticalLayout());
		math::Recti scrollRect(rect.upperLeftCorner.x, rect.lowerRightCorner.y,
			rect.lowerRightCorner.x, rect.lowerRightCorner.y + rect.GetHeight() + 200);
		m_scrollWidget.reset(new ScrollWidget());
		m_scrollWidget->SetLayout(m_layout);
		m_scrollWidget->SetRect(scrollRect);
	}

	void ComboBox::Render(video::Painter* painter)
	{
		if (m_isInFocus)
			return;

		m_button->Render(painter);
		if (m_showList)
		{
			m_scrollWidget->Render(painter);
		}
	}

	void ComboBox::SetPosition(s32 x, s32 y)
	{
		m_button->SetPosition(x, y);
		m_scrollWidget->SetPosition(x, y + m_button->GetRect().GetHeight());
	}

	void ComboBox::SetSize(const math::Vector2i& size)
	{
		m_button->SetSize(size);
		m_scrollWidget->SetWidth(size.x);
	}

	void ComboBox::SetWidth(s32 width)
	{
		m_button->SetWidth(width);
		m_scrollWidget->SetWidth(width);
	}

	void ComboBox::SetHeight(s32 height)
	{
		m_button->SetHeight(height);
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
		m_layout->AddWidget(button);
		
		
		u32 childrenCount = m_layout->GetItemsCount();

		if (childrenCount == 1U)
		{
			m_button->SetText(text);
			m_currentSelectedItem = button;
			m_currentSelectedItem->SetToggled(true);
		}

		//m_scrollWidget->SetRect(math::Recti(x0, y0, x1, y1));
		m_scrollWidget->UpdateLayout();
	}

	void ComboBox::RemoveItem(const String& text)
	{
		u32 itemsCount = m_layout->GetItemsCount();
		for (u32 i = 0U; i < itemsCount; ++i)
		{
			const auto& button = std::static_pointer_cast<Button>(m_layout->GetWidget(i));
			if (button->GetText() == text)
			{
				m_layout->RemoveWidget(button);
				m_scrollWidget->UpdateLayout();
				return;
			}
		}
	}

	void ComboBox::RemoveItem(u32 index)
	{
		u32 itemsCount = m_layout->GetItemsCount();
		if (index >= itemsCount)
			return;

		const auto& widget = m_layout->GetWidget(index);
		m_layout->RemoveWidget(widget);
		m_scrollWidget->UpdateLayout();
	}

	void ComboBox::SetSelectedItem(const String& item)
	{

	}

	void ComboBox::SetSelectedItem(const u32 index)
	{
		if (index >= m_layout->GetItemsCount())
			return;

		const auto& button = std::static_pointer_cast<Button>(m_layout->GetWidget(index));
		button->SetToggled(true);
	}

	void ComboBox::OnButtonToggled(bool toggled)
	{
		m_showList = toggled;
		m_isInFocus = toggled;

		if (m_isInFocus)
		{
			GuiManager::GetInstance()->SetFocusWidget(shared_from_this());
		}
		else
		{
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

		u32 itemsCount = m_layout->GetItemsCount();
		for (u32 i = 0U; i < itemsCount; ++i)
		{
			const auto& button = std::static_pointer_cast<Button>(m_layout->GetWidget(i));
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
		u32 childrenCount = m_layout->GetItemsCount();
		u32 x0 = m_button->GetPosition().x;
		u32 y0 = m_button->GetRect().lowerRightCorner.y;
		u32 x1 = x0 + 150;
		u32 y1 = y0 + 15 * childrenCount;
		//m_layout->Resize(math::Recti(x0, y0, x1, y1));
	}

} // gui

} // sh