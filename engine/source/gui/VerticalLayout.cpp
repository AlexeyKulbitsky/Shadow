#include "VerticalLayout.h"

namespace sh
{
	
namespace gui
{

	void VerticalLayout::AddWidget(const WidgetPtr& widget)
	{
		Layout::AddWidget(widget);
	}

	void VerticalLayout::RemoveWidget(const WidgetPtr& widget)
	{
		Layout::RemoveWidget(widget);
	}

	void VerticalLayout::AddLayout(const LayoutPtr& layout)
	{
		Layout::AddLayout(layout);
	}

	void VerticalLayout::Resize(const math::Recti& rect)
	{
		if (m_items.size() == 0U)
			return;

		const s32 height = rect.GetHeight();
		const s32 width = rect.GetWidth();
		const u32 elementCount = m_items.size();
		s32 itemHeight = height / elementCount;
		s32 offset = 0;

		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const s32 x = rect.upperLeftCorner.x;
			const s32 y = rect.upperLeftCorner.y + offset;
			m_items[i]->Resize(math::Recti(x, y, x + width, y + itemHeight));
			offset += m_items[i]->GetHeight();
		}
		m_rect.upperLeftCorner = rect.upperLeftCorner;
		m_rect.lowerRightCorner = rect.upperLeftCorner + math::Vector2i(width, offset);
	}

	void VerticalLayout::SetSize(const math::Vector2i& size)
	{

	}

	void VerticalLayout::SetWidth(s32 width)
	{

	}

	void VerticalLayout::SetHeight(s32 height)
	{
		const u32 elementCount = m_items.size();
		const u32 itemHeight = height / elementCount;

		for (const auto& item : m_items)
		{
			// If this item if for holding widget
			const auto& w = item->GetWidget();
			if (w)
			{
				w->SetHeight(itemHeight);
				continue;
			}

			// If this item is layout itself
			item->SetHeight(itemHeight);
		}
	}

} // gui

} // sh