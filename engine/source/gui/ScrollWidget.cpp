#include "ScrollWidget.h"
#include "Layout.h"

#include "../video/Painter.h"
#include "../Device.h"
#include "../video/Driver.h"

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
		painter->SetClipRect(math::Rectu(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y,
			m_rect.lowerRightCorner.x, m_rect.lowerRightCorner.y));
		Widget::Render(painter);
		painter->SetClipRect(Device::GetInstance()->GetDriver()->GetViewport());
	}

	void ScrollWidget::SetPosition(s32 x, s32 y)
	{
		Widget::SetPosition(x, y);
	}

	void ScrollWidget::SetSize(const math::Vector2i& size)
	{
		Widget::SetSize(size);
	}

	void ScrollWidget::SetWidth(s32 width)
	{
		Widget::SetWidth(width);
	}

	void ScrollWidget::SetHeight(s32 height)
	{
		Widget::SetHeight(height);
	}

	bool ScrollWidget::ProcessEvent(GUIEvent& ev)
	{
		bool inside = m_rect.IsPointInside(ev.x, ev.y);
		if (!inside)
			return false;

		switch (ev.type)
		{
		case EventType::Wheel:
		{
			int delta = ev.delta > 0 ? 15 : -15;
			m_fullRect.lowerRightCorner.y += delta;
			m_fullRect.upperLeftCorner.y += delta;
			if (m_fullRect.upperLeftCorner.y > m_rect.upperLeftCorner.y)
			{
				int d = m_rect.upperLeftCorner.y - m_fullRect.upperLeftCorner.y;
				m_fullRect.lowerRightCorner.y += d;
				m_fullRect.upperLeftCorner.y += d;
			}

			if (m_fullRect.lowerRightCorner.y < m_rect.lowerRightCorner.y)
			{
				int d = m_rect.lowerRightCorner.y - m_fullRect.lowerRightCorner.y;
				m_fullRect.lowerRightCorner.y += d;
				m_fullRect.upperLeftCorner.y += d;
			}

			m_layout->Resize(m_fullRect);
		}
		break;
		default:
		{
			return Widget::ProcessEvent(ev);
		}
			break;
		}
		return false;
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
			m_fullRect = r;
		}
	}


} // gui

} // sh