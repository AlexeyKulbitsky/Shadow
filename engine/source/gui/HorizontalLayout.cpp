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
		const u32 height = rect.GetHeight();
		const u32 width = rect.GetWidth();

		const u32 elementCount = m_items.size();
		u32 itemWidth = width / elementCount;
		u32 offset = 0U;

		for (u32 i = 0; i < m_items.size(); ++i)
		{
			// If this item if for holding widget
			const auto& w = m_items[i]->GetWidget();
			const u32 x = rect.upperLeftCorner.x + offset;// itemWidth * i;
			const u32 y = rect.upperLeftCorner.y;
			if (w)
			{
				w->SetPosition(x, y);
				w->SetHeight(height);

				u32 maxWidth = w->GetMaximumWidth();
				if (maxWidth != 0U)
				{
					if (maxWidth < itemWidth)
					{
						w->SetWidth(maxWidth - 1);
						itemWidth = (width - offset - maxWidth) / (elementCount - i - 1);
						offset += maxWidth;
					}
					else
					{
						w->SetWidth(itemWidth - 1);
						offset += itemWidth;
					}
				}
				else
				{
					w->SetWidth(itemWidth - 1);
					offset += itemWidth;
				}
				continue;
			}

			// If this item is layout itself
			m_items[i]->Resize(math::Recti(x, y, x + itemWidth, y + height));
			offset += itemWidth;
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