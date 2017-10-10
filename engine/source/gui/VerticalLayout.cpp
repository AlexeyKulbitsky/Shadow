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

		auto upperLeft = rect.upperLeftCorner;
		auto lowerRight = rect.lowerRightCorner;

		math::Recti finalRect;
		finalRect.upperLeftCorner.x = upperLeft.x + static_cast<s32>(m_leftMargin);
		finalRect.upperLeftCorner.y = upperLeft.y + static_cast<s32>(m_topMargin);
		finalRect.lowerRightCorner.x = lowerRight.x - static_cast<s32>(m_rightMargin);
		finalRect.lowerRightCorner.y = lowerRight.y - static_cast<s32>(m_bottomMargin);

		const s32 height = finalRect.GetHeight();
		const s32 width = finalRect.GetWidth();
		const u32 elementCount = m_items.size();
		s32 itemHeight = 0;
		s32 offset = 0;

		for (u32 i = 0; i < m_items.size(); ++i)
		{
			if (offset > height)
			{
				itemHeight = 0;
			}
			else
			{
				const s32 temp = static_cast<s32>(elementCount) - static_cast<s32>(i);
				itemHeight = (height - offset - m_spacing * (temp - 1)) / (temp);
			}
			const s32 x = finalRect.upperLeftCorner.x;
			const s32 y = finalRect.upperLeftCorner.y + offset;
			m_items[i]->Resize(math::Recti(x, y, x + width, y + itemHeight));
			if (m_items[i]->GetHeight() != 0)
				offset += m_items[i]->GetHeight() + m_spacing;
		}
		m_rect.upperLeftCorner = finalRect.upperLeftCorner;
		m_rect.lowerRightCorner = finalRect.upperLeftCorner + math::Vector2i(width, offset);
	}

	void VerticalLayout::SetSize(const math::Vector2i& size)
	{

	}

	void VerticalLayout::SetWidth(s32 width)
	{
		m_rect.lowerRightCorner.x = m_rect.upperLeftCorner.x + width;
		Resize(m_rect);
	}

	void VerticalLayout::SetHeight(s32 height)
	{
		m_rect.lowerRightCorner.y = m_rect.upperLeftCorner.y + height;
		Resize(m_rect);
	}

} // gui

} // sh