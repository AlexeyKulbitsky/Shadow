#include "HorizontalLayout.h"

namespace sh
{
	
namespace gui
{

	void HorizontalLayout::AddWidget(const WidgetPtr& widget)
	{
		Layout::AddWidget(widget);
	}

	void HorizontalLayout::RemoveWidget(const WidgetPtr& widget)
	{
		Layout::RemoveWidget(widget);
	}

	void HorizontalLayout::AddLayout(const LayoutPtr& layout)
	{
		Layout::AddLayout(layout);
	}

	void HorizontalLayout::Resize(const math::Recti& rect)
	{
		if (m_items.size() == 0U)
			return;

		const s32 height = rect.GetHeight();
		const s32 width = rect.GetWidth();
		const u32 elementCount = m_items.size();
		s32 itemWidth = width / elementCount;
		s32 offset = 0U;

		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const s32 x = rect.upperLeftCorner.x + offset;
			const s32 y = rect.upperLeftCorner.y;
			m_items[i]->Resize(math::Recti(x, y, x + itemWidth, y));
			offset += m_items[i]->GetWidth();
		}
		m_rect.upperLeftCorner = rect.upperLeftCorner;
		m_rect.lowerRightCorner = rect.upperLeftCorner + math::Vector2i(width, offset);
	}

	void HorizontalLayout::SetSize(const math::Vector2i& size)
	{
		Layout::SetSize(size);
	}

	void HorizontalLayout::SetWidth(s32 width)
	{
		const u32 elementCount = m_items.size();
		const u32 itemWidth = width / elementCount;

		for (const auto& item : m_items)
		{
			// If this item if for holding widget
			const auto& w = item->GetWidget();
			if (w)
			{
				w->SetWidth(width);
				continue;
			}

			// If this item is layout itself
			item->SetWidth(width);
		}
	}

	void HorizontalLayout::SetHeight(s32 height)
	{
		Layout::SetHeight(height);
	}

} // gui

} // sh