#include "ComboBox.h"

namespace sh
{

namespace gui
{

	ComboBox::ComboBox()
		: ComboBox(math::Rectu(0U, 0U, 10U, 10U))
	{
	}

	ComboBox::ComboBox(const math::Rectu& rect)
	{
		m_button.reset(new Button(rect));
		m_button->SetToggleable(true);
		m_button->OnToggle.Connect(std::bind(&ComboBox::OnButtonToggled, this, std::placeholders::_1));

		m_layout.reset(new VerticalLayout());
	}

	void ComboBox::GetGeometry(GuiBatchData& data)
	{
		m_button->GetGeometry(data);

		if (m_showList)
			m_layout->GetGeometry(data);
	}

	void ComboBox::GetTextGeometry(GuiBatchData& data)
	{
		m_button->GetTextGeometry(data);

		if (m_showList)
			m_layout->GetTextGeometry(data);
	}

	void ComboBox::SetPosition(u32 x, u32 y)
	{
		m_button->SetPosition(x, y);
		Widget::SetPosition(x, y);
	}

	void ComboBox::SetSize(const math::Vector2u& size)
	{
		m_button->SetSize(size);
		Widget::SetSize(size);
	}

	void ComboBox::SetWidth(u32 width)
	{
		m_button->SetWidth(width);
		Widget::SetWidth(width);
	}

	void ComboBox::SetHeight(u32 height)
	{
		m_button->SetHeight(height);
		Widget::SetHeight(height);
	}

	bool ComboBox::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		if (!m_button->ProcessInput(x, y, type))
		{
			if (m_showList)
				return m_layout->ProcessInput(x, y, type);
		}
		return false;
	}

	void ComboBox::AddItem(const String& text)
	{
		ButtonPtr button(new Button(text));
		button->SetToggleable(true);
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
		u32 x0 = m_button->GetPosition().x;
		u32 y0 = m_button->GetRect().lowerRightCorner.y;
		u32 x1 = x0 + 150U;
		u32 y1 = y0 + 15U * childrenCount;

		m_layout->Resize(math::Rectu(x0, y0, x1, y1));
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
				UpdateLayout();
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
		UpdateLayout();
	}

	void ComboBox::OnButtonToggled(bool toggled)
	{
		m_showList = toggled;
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
		u32 x1 = x0 + 150U;
		u32 y1 = y0 + 15U * childrenCount;
		m_layout->Resize(math::Rectu(x0, y0, x1, y1));
	}

} // gui

} // sh