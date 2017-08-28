#include "Widget.h"
#include "Layout.h"
#include "GuiManager.h"

#include "../Device.h"

namespace sh
{

namespace gui
{

	Widget::Widget()
	{
		m_rect.lowerRightCorner = math::Vector2i(10, 10);
	}

	Widget::~Widget()
	{
	}

	void Widget::SetLayout(const LayoutPtr& layout)
	{
		m_layout = layout;
		m_layout->SetParent(this);
		UpdateLayout();
	}

	void Widget::Load(const pugi::xml_node& node)
	{
	}

	void Widget::Render(video::Painter* painter)
	{
		if (m_layout)
			m_layout->Render(painter);
	}

	void Widget::SetPosition(s32 x, s32 y)
	{
		auto size = m_rect.GetSize();
		m_rect.Set(x, y, x + size.x, y + size.y);
		UpdateLayout();
	}

	void Widget::SetSize(const math::Vector2i& size)
	{
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		UpdateLayout();
	}

	void Widget::SetWidth(s32 width)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + width, pos.y + size.y);
		UpdateLayout();
	}

	void Widget::SetHeight(s32 height)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + height);
		UpdateLayout();
	}

	bool Widget::ProcessEvent(GUIEvent& ev)
	{
		if (!m_layout)
			return false;

		return m_layout->ProcessEvent(ev);
	}

	void Widget::SetMargins(u32 top, u32 right, u32 bottom, u32 left)
	{
		m_topMargin = top;
		m_rightMargin = right;
		m_bottomMargin = bottom;
		m_leftMargin = left;

		UpdateLayout();
	}

	void Widget::UpdateLayout()
	{
		if (m_layout)
		{
			auto upperLeft = m_rect.upperLeftCorner;
			auto lowerRight = m_rect.lowerRightCorner;

			math::Recti finalRect;
			finalRect.upperLeftCorner.x = upperLeft.x + m_leftMargin;
			finalRect.upperLeftCorner.y = upperLeft.y + m_topMargin;
			finalRect.lowerRightCorner.x = lowerRight.x - m_rightMargin;
			finalRect.lowerRightCorner.y = lowerRight.y - m_bottomMargin;

			m_layout->Resize(finalRect);
		}
	}

} // gui

} // sh