#include "Layout.h"

namespace sh
{

namespace gui
{

	LayoutItem::LayoutItem(const WidgetPtr& widget)
		: m_widget(widget)
	{

	}

	const WidgetPtr& LayoutItem::GetWidget() const
	{
		return m_widget;
	}

	////////////////////////////////////////////////////////////////////

	void Layout::AddWidget(const WidgetPtr& widget)
	{
		LayoutItemPtr item(new LayoutItem(widget));
		m_items.push_back(item);

		if (m_parent)
			m_parent->UpdateLayout();
	}

	void Layout::RemoveWidget(const WidgetPtr& widget)
	{
		for (auto item = m_items.begin(); item != m_items.end(); ++item)
		{
			if ((*item)->GetWidget() == widget)
			{
				m_items.erase(item);
				return;
			}
		}
	}

	void Layout::Clear()
	{
		m_items.clear();
	}

	void Layout::AddLayout(const LayoutPtr& layout)
	{
		m_items.push_back(layout);

		if (m_parent)
			m_parent->UpdateLayout();
	}

	void Layout::SetSize(const math::Vector2u& size)
	{

	}

	void Layout::SetWidth(u32 width)
	{

	}

	void Layout::SetHeight(u32 height)
	{

	}

	void Layout::Render(video::Painter* painter)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget)
			{
				widget->Render(painter);
				continue;
			}

			m_items[i]->Render(painter);
		}
	}

	bool Layout::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget)
			{
				if (widget->ProcessInput(x, y, type))
					return true;
			}

			if (m_items[i]->ProcessInput(x, y, type))
				return true;
		}

		return false;
	}

	bool Layout::ProcessKeyboardInput(KeyboardEventType type, KeyCode code)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget)
			{
				if (widget->ProcessKeyboardInput(type, code))
					return true;
			}

			if (m_items[i]->ProcessKeyboardInput(type, code))
				return true;
		}

		return false;
	}


} // gui

} // sh