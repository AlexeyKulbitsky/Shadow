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
	}

	void Layout::AddLayout(const LayoutPtr& layout)
	{
		m_items.push_back(layout);
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

	void Layout::GetGeometry(GuiBatchData& data)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget)
			{
				widget->GetGeometry(data);
				continue;
			}

			m_items[i]->GetGeometry(data);
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


} // gui

} // sh