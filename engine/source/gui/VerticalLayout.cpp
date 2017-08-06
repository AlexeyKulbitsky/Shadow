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

	void VerticalLayout::Resize(const math::Rectu& rect)
	{
		if (m_items.size() == 0U)
			return;

		const u32 height = rect.GetHeight();
		const u32 width = rect.GetWidth();
		

		const u32 elementCount = m_items.size();
		u32 itemHeight = height / elementCount;
		u32 offset = 0U;

		for (u32 i = 0; i < m_items.size(); ++i)
		{
			// If this item if for holding widget
			const auto& w = m_items[i]->GetWidget();
			const u32 x = rect.upperLeftCorner.x;
			const u32 y = rect.upperLeftCorner.y + offset;// itemHeight * i;
			if (w)
			{
				w->SetPosition(x, y);
				w->SetWidth(width);
				const u32 maxHeight = w->GetMaximumHeight();
				if (maxHeight != 0U)
				{
					if (maxHeight < itemHeight)
					{
						w->SetHeight(maxHeight);
						u32 den = elementCount - i - 1;
						if (den == 0U)
							continue;
						itemHeight = (height - offset - maxHeight) / den;
						offset += maxHeight;
					}
					else
					{
						w->SetHeight(itemHeight);
						offset += itemHeight;
					}
				}
				else
				{
					w->SetHeight(itemHeight);
					offset += itemHeight;
				}
				
				
				continue;
			}

			// If this item is layout itself
			m_items[i]->Resize(math::Rectu(x, y, x + width, y + itemHeight));
			offset += itemHeight;
		}
	}

	void VerticalLayout::SetSize(const math::Vector2u& size)
	{

	}

	void VerticalLayout::SetWidth(u32 width)
	{

	}

	void VerticalLayout::SetHeight(u32 height)
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