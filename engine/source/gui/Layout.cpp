#include "Layout.h"

namespace sh
{

namespace gui
{

	void WidgetItem::Resize(const math::Recti& rect)
	{
		m_widget->SetPosition(rect.upperLeftCorner.x, rect.upperLeftCorner.y);
		s32 width = rect.GetWidth();
		s32 height = rect.GetHeight();
		
		if (width < m_widget->GetMinimumWidth())
			width = m_widget->GetMinimumWidth();
		if (width > m_widget->GetMaximumWidth())
			width = m_widget->GetMaximumWidth();

		if (height < m_widget->GetMinimumHeight())
			height = m_widget->GetMinimumHeight();
		if (height > m_widget->GetMaximumHeight())
			height = m_widget->GetMaximumHeight();

		m_widget->SetWidth(width);
		m_widget->SetHeight(height);
	}

	////////////////////////////////////////////////////////////////////

	void Layout::AddWidget(const WidgetPtr& widget)
	{
		LayoutItemPtr item(new WidgetItem(widget));
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

	void Layout::SetSize(const math::Vector2i& size)
	{
		
	}

	void Layout::SetWidth(s32 width)
	{

	}

	void Layout::SetHeight(s32 height)
	{

	}

	void Layout::SetMargins(u32 top, u32 right, u32 bottom, u32 left)
	{
		m_topMargin = top;
		m_rightMargin = right;
		m_bottomMargin = bottom;
		m_leftMargin = left;
	}

	void Layout::Render(video::Painter* painter)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget && widget->IsVisible())
			{
				widget->Render(painter);
				continue;
			}

			m_items[i]->Render(painter);
		}
	}

	bool Layout::ProcessEvent(GUIEvent& ev)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget)
			{
				if (widget->ProcessEvent(ev))
					return true;
			}

			if (m_items[i]->ProcessEvent(ev))
				return true;
		}

		return false;
	}


} // gui

} // sh