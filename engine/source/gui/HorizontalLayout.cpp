#include "HorizontalLayout.h"

namespace sh
{
	
namespace gui
{

	void HorizontalLayout::AddWidget(const WidgetPtr& widget)
	{
		Layout::AddWidget(widget);
	}

	void HorizontalLayout::AddLayout(const LayoutPtr& layout)
	{
		Layout::AddLayout(layout);
	}

	void HorizontalLayout::Resize(const math::Rectu& rect)
	{
		const u32 height = rect.GetHeight();
		const u32 width = rect.GetWidth();

		const u32 elementCount = m_items.size();
		const u32 itemWidth = width / elementCount;

		for (u32 i = 0; i < m_items.size(); ++i)
		{
			// If this item if for holding widget
			const auto& w = m_items[i]->GetWidget();
			if (w)
			{
				w->SetPosition(rect.upperLeftCorner.x + itemWidth * i, rect.upperLeftCorner.y);
				w->SetHeight(height);
				w->SetWidth(itemWidth - 5);
				continue;
			}

			// If this item is layout itself
			m_items[i]->SetHeight(height);
			m_items[i]->SetWidth(itemWidth);
		}
	}

	void HorizontalLayout::SetSize(const math::Vector2u& size)
	{
		Layout::SetSize(size);
	}

	void HorizontalLayout::SetWidth(u32 width)
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

	void HorizontalLayout::SetHeight(u32 height)
	{
		Layout::SetHeight(height);
	}

} // gui

} // sh