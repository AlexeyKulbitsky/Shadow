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

	void HorizontalLayout::Resize(const math::Rect& rect)
	{
		if (m_items.size() == 0U)
			return;

		auto upperLeft = rect.upperLeftCorner;
		auto lowerRight = rect.lowerRightCorner;

		math::Rect finalRect;
		finalRect.upperLeftCorner.x = upperLeft.x + m_leftMargin;
		finalRect.upperLeftCorner.y = upperLeft.y + m_topMargin;
		finalRect.lowerRightCorner.x = lowerRight.x - m_rightMargin;
		finalRect.lowerRightCorner.y = lowerRight.y - m_bottomMargin;

		const s32 height = finalRect.GetHeight();
		const s32 width = finalRect.GetWidth();
		const u32 elementCount = m_items.size();
		s32 itemWidth = 0;
		s32 offset = 0;

		for (u32 i = 0; i < m_items.size(); ++i)
		{
			if (offset > width)
				itemWidth = 0;
			else
				itemWidth = (width - offset - m_spacing * (elementCount - i - 1)) / (elementCount - i);
			const s32 x = finalRect.upperLeftCorner.x + offset;
			const s32 y = finalRect.upperLeftCorner.y;
			m_items[i]->Resize(math::Rect(x, y, x + itemWidth, y + height));
			offset += m_items[i]->GetWidth() + m_spacing;
		}


		m_rect.upperLeftCorner = finalRect.upperLeftCorner;
		m_rect.lowerRightCorner = finalRect.upperLeftCorner + math::Vector2Int(offset, height);
	}

	void HorizontalLayout::SetSize(const math::Vector2Int& size)
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