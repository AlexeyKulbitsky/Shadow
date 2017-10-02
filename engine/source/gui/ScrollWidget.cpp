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
		if (!m_visible)
			return;

		painter->SetClipRect(math::Rectu(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y,
			m_rect.lowerRightCorner.x, m_rect.lowerRightCorner.y));
		Widget::Render(painter);
		painter->SetClipRect(Device::GetInstance()->GetDriver()->GetViewport());
	}

	void ScrollWidget::SetPosition(s32 x, s32 y)
	{
		const s32 newX = m_fullRect.upperLeftCorner.x + (x - m_rect.upperLeftCorner.x);
		const s32 newY = m_fullRect.upperLeftCorner.y + (y - m_rect.upperLeftCorner.y);
		auto size = m_fullRect.GetSize();
		m_fullRect.Set(newX, newY, newX + size.x, newY + size.y);
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
			const s32 fullRectHeight = m_fullRect.GetHeight();
			const s32 rectHeight = m_rect.GetHeight();
			if (fullRectHeight < rectHeight)
				return true;

			int delta = ev.delta > 0 ? m_scrollSpeed : -m_scrollSpeed;
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

			return true;
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
			if (itemsCount > 1)
			{
				height += m_layout->GetSpacing() * (itemsCount - 1);
			}
			for (u32 i = 0U; i < itemsCount; ++i)
			{
				height += m_layout->GetItem(i)->GetHeight();
			}
			auto r = m_rect;
			r.lowerRightCorner.y = r.upperLeftCorner.y + height;
			m_layout->Resize(r);
			m_fullRect = r;
		}
	}

	void ScrollWidget::UpdateScrollBars()
	{
	}


} // gui

} // sh