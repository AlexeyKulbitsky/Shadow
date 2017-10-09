#include "ScrollWidget.h"
#include "Layout.h"
#include "GuiManager.h"
#include "Sprite.h"
#include "Style.h"

#include "../video/Painter.h"
#include "../Device.h"
#include "../video/Driver.h"

namespace sh
{

namespace gui
{
	
	ScrollWidget::ScrollBar::ScrollBar()
		: Button()
	{
	}

	ScrollWidget::ScrollBar::ScrollBar(const SpritePtr& defaultSprite,
		const SpritePtr& pressedSprite,
		const SpritePtr& hoveredSprite,
		const SpritePtr& backgroundSprite)
		: Button(defaultSprite, pressedSprite, hoveredSprite)
		, m_backgroundSprite(backgroundSprite)
	{
	}

	bool ScrollWidget::ScrollBar::ProcessEvent(sh::gui::GUIEvent& ev)
	{
		bool inside = m_rect.IsPointInside(ev.x, ev.y);

		if (ev.mouseButtonCode != MouseCode::ButtonLeft)
			return false;

		Button::ProcessEvent(ev);

		switch (ev.type)
		{
		case sh::gui::EventType::PointerDown:
		{
			if (inside)
			{
				m_startPos.x = ev.x;
				m_startPos.y = ev.y;
				m_dragStarted = true;
				return true;
			}
		}
		break;
		case sh::gui::EventType::PointerUp:
		{
			m_dragStarted = false;
			return true;
		}
		break;
		case sh::gui::EventType::PointerMove:
		{
			if (m_dragStarted)
			{
				sh::Device* device = sh::Device::GetInstance();
				sh::InputManager* inputManager = device->GetInputManager();

				if (inputManager->IsMouseButtonPressed(sh::MouseCode::ButtonLeft))
				{
					int yDelta = ev.y - m_startPos.y;
					m_startPos.y = ev.y;

					if (m_rect.upperLeftCorner.y + yDelta < m_scrollWidget->GetRect().upperLeftCorner.y)
					{
						yDelta = m_scrollWidget->GetRect().upperLeftCorner.y - m_rect.upperLeftCorner.y;
					}
					if (m_rect.upperLeftCorner.y + yDelta > m_scrollWidget->GetRect().lowerRightCorner.y - m_rect.GetHeight())
					{
						yDelta = m_scrollWidget->GetRect().lowerRightCorner.y - m_rect.GetHeight() - m_rect.upperLeftCorner.y;
					}

					m_rect.upperLeftCorner.y += yDelta;
					m_rect.lowerRightCorner.y += yDelta;

					const float deltaYAspect = static_cast<float>(yDelta) / static_cast<float>(m_scrollWidget->m_rect.GetHeight() - m_rect.GetHeight());
					const float deltaYFloat = deltaYAspect * static_cast<float>(m_scrollWidget->m_fullRect.GetHeight() - m_scrollWidget->m_rect.GetHeight());
					const int deltaYFinal = static_cast<int>(deltaYFloat);

					m_scrollWidget->m_fullRect.upperLeftCorner.y -= deltaYFinal;
					m_scrollWidget->m_fullRect.lowerRightCorner.y -= deltaYFinal;
					m_scrollWidget->m_layout->Resize(m_scrollWidget->m_fullRect);

					return true;
				}
			}
		}
		break;
		}

		return false;
	}

	void ScrollWidget::ScrollBar::Render(video::Painter* painter)
	{
		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());

		auto rect = m_scrollWidget->GetRect();

		rect.upperLeftCorner.x = rect.lowerRightCorner.x - m_rect.GetWidth();

		video::Painter::Vertex upperLeft(rect.upperLeftCorner,
			m_backgroundSprite->GetUVRect().upperLeftCorner,
			m_backgroundSprite->GetColor());
		video::Painter::Vertex downRight(rect.lowerRightCorner,
			m_backgroundSprite->GetUVRect().lowerRightCorner,
			m_backgroundSprite->GetColor());
		painter->DrawRect(upperLeft, downRight);
		
		// Draw bar
		{
			SpritePtr sprite;
			if (m_dragStarted)
			{
				sprite = m_sprites[Button::Pressed];
			}
			else
			{
				sprite = m_sprites[m_state];
			}
			video::Painter::Vertex upperLeft(m_rect.upperLeftCorner,
				sprite->GetUVRect().upperLeftCorner,
				sprite->GetColor());
			video::Painter::Vertex downRight(m_rect.lowerRightCorner,
				sprite->GetUVRect().lowerRightCorner,
				sprite->GetColor());
			painter->DrawRect(upperLeft, downRight);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////

	ScrollWidget::ScrollWidget()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetScrollWidget();
		m_verticalScrollBar.reset(new ScrollBar(
			ref->m_verticalScrollBar->m_sprites[Button::Released],
			ref->m_verticalScrollBar->m_sprites[Button::Pressed],
			ref->m_verticalScrollBar->m_sprites[Button::Hovered],
			ref->m_verticalScrollBar->m_backgroundSprite));
		m_verticalScrollBar->m_scrollWidget = this;
	}

	ScrollWidget::ScrollWidget(const SPtr<ScrollBar>& scrollBar)
	{
		m_verticalScrollBar.reset(new ScrollBar(
			scrollBar->m_sprites[Button::Released],
			scrollBar->m_sprites[Button::Pressed],
			scrollBar->m_sprites[Button::Hovered],
			scrollBar->m_backgroundSprite));
		m_verticalScrollBar->m_scrollWidget = this;
	}

	ScrollWidget::~ScrollWidget()
	{

	}

	void ScrollWidget::Render(video::Painter* painter)
	{
		if (!m_visible)
			return;

		const auto cachedClipRect = painter->GetClipRect();
		painter->SetClipRect(math::Rectu(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y,
			m_rect.lowerRightCorner.x, m_rect.lowerRightCorner.y));
		Widget::Render(painter);
		painter->SetClipRect(cachedClipRect);

		RenderScrollBars(painter);
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

		const auto fullHeight = m_fullRect.GetHeight();
		const auto visibleHeight = m_rect.GetHeight();

		if (fullHeight > visibleHeight)
		{
			if (m_verticalScrollBar->ProcessEvent(ev))
				return true;
		}


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

	void ScrollWidget::RenderScrollBars(video::Painter* painter)
	{
		const auto fullHeight = m_fullRect.GetHeight();
		const auto visibleHeight = m_rect.GetHeight();

		if (fullHeight > visibleHeight)
		{
			const float aspect = static_cast<float>(visibleHeight) / static_cast<float>(fullHeight);
			const auto scrollBarHeight = static_cast<sh::s32>(aspect * static_cast<float>(visibleHeight));
			const float yPosAspect = static_cast<float>(m_rect.upperLeftCorner.y - m_fullRect.upperLeftCorner.y)
				/ static_cast<float>(fullHeight - visibleHeight);
			const s32 yPos = m_rect.upperLeftCorner.y + static_cast<s32>(yPosAspect * (visibleHeight - scrollBarHeight));

			const s32 scrollBarWidth = 20;

			m_verticalScrollBar->SetPosition(m_rect.lowerRightCorner.x - scrollBarWidth, yPos);
			m_verticalScrollBar->SetWidth(scrollBarWidth);
			m_verticalScrollBar->SetHeight(scrollBarHeight);
			m_verticalScrollBar->Render(painter);
		}
	}


} // gui

} // sh