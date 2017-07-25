#include "VerticalLayout.h"

namespace sh
{
	
namespace gui
{

	void VerticalLayout::AddWidget(const WidgetPtr& widget)
	{
		Layout::AddWidget(widget);
	}

	void VerticalLayout::AddLayout(const LayoutPtr& layout)
	{
		Layout::AddLayout(layout);
	}

	void VerticalLayout::Resize(const math::Rectu& rect)
	{
		const u32 height = rect.GetHeight();
		const u32 width = rect.GetWidth();
		

		const u32 elementCount = m_items.size();
		const u32 itemHeight = height / elementCount;

		for (u32 i = 0; i < m_items.size(); ++i)
		{
			// If this item if for holding widget
			const auto& w = m_items[i]->GetWidget();
			const u32 x = rect.upperLeftCorner.x;
			const u32 y = rect.upperLeftCorner.y + itemHeight * i;
			if (w)
			{
				w->SetPosition(x, y);
				w->SetHeight(itemHeight);
				w->SetWidth(width);
				continue;
			}

			// If this item is layout itself
			m_items[i]->Resize(math::Rectu(x, y, x + width, y + itemHeight));
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