#include "ScrollWidget.h"
#include "Layout.h"

namespace sh
{

namespace gui
{

	ScrollWidget::ScrollWidget()
	{

	}

	ScrollWidget::~ScrollWidget()
	{

	}

	void ScrollWidget::Render(video::Painter* painter)
	{
		Widget::Render(painter);
	}

	void ScrollWidget::SetPosition(u32 x, u32 y)
	{
		Widget::SetPosition(x, y);
	}

	void ScrollWidget::SetSize(const math::Vector2u& size)
	{
		Widget::SetSize(size);
	}

	void ScrollWidget::SetWidth(u32 width)
	{
		Widget::SetWidth(width);
	}

	void ScrollWidget::SetHeight(u32 height)
	{
		Widget::SetHeight(height);
	}

	void ScrollWidget::UpdateLayout()
	{
		if (m_layout)
		{
			const u32 itemsCount = m_layout->GetItemsCount();
			u32 height = 0U;
			for (u32 i = 0U; i < itemsCount; ++i)
			{
				const auto& w = m_layout->GetWidget(i);
				if (!w)
					continue;
				height += w->GetRect().GetHeight();
			}
			auto r = m_rect;
			r.lowerRightCorner.y = r.upperLeftCorner.y + height;
			m_layout->Resize(r);
		}
	}


} // gui

} // sh