#include "Widget.h"

#include "Layout.h"

namespace sh
{

namespace gui
{

	Widget::Widget()
	{
		m_rect.lowerRightCorner = math::Vector2u(10U, 10U);
	}

	Widget::~Widget()
	{
	}

	void Widget::SetLayout(const LayoutPtr& layout)
	{
		m_layout = layout;
		UpdateLayout();
	}

	void Widget::Load(const pugi::xml_node& node)
	{
		GuiElement::Load(node);
	}

	void Widget::GetGeometry(GuiBatchData& data)
	{
		//GuiElement::GetGeometry(data);
		if (!m_layout)
			return;

		m_layout->GetGeometry(data);
	}

	void Widget::GetTextGeometry(GuiBatchData& data)
	{
		//GuiElement::GetTextGeometry(data);
		if (!m_layout)
			return;

		m_layout->GetTextGeometry(data);
	}

	void Widget::SetPosition(u32 x, u32 y)
	{
		GuiElement::SetPosition(x, y);
		UpdateLayout();
	}

	void Widget::SetSize(const math::Vector2u& size)
	{
		GuiElement::SetSize(size);
		UpdateLayout();
	}

	void Widget::SetWidth(u32 width)
	{
		GuiElement::SetWidth(width);
		UpdateLayout();
	}

	void Widget::SetHeight(u32 height)
	{
		GuiElement::SetHeight(height);
		UpdateLayout();
	}

	bool Widget::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		//return GuiElement::ProcessInput(x, y, type);

		if (!m_layout)
			return false;

		return m_layout->ProcessInput(x, y, type);
	}

	bool Widget::ProcessKeyboardInput(KeyboardEventType type, KeyCode code)
	{
		//return GuiElement::ProcessKeyboardInput(type, code);

		if (!m_layout)
			return false;

		return m_layout->ProcessKeyboardInput(type, code);
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

			math::Rectu finalRect;
			finalRect.upperLeftCorner.x = upperLeft.x + m_leftMargin;
			finalRect.upperLeftCorner.y = upperLeft.y + m_topMargin;
			finalRect.lowerRightCorner.x = lowerRight.x - m_rightMargin;
			finalRect.lowerRightCorner.y = lowerRight.y - m_bottomMargin;

			m_layout->Resize(finalRect);
		}
	}


} // gui

} // sh