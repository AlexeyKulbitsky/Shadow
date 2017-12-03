#include "Widget.h"
#include "Layout.h"
#include "GuiManager.h"

#include "../Device.h"

#include <pugixml.hpp>

namespace sh
{

namespace gui
{

	Widget::Widget()
	{
		m_rect.lowerRightCorner = math::Vector2Int(10, 10);
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

	void Widget::RenderBackground(video::Painter* painter)
	{
		if (m_layout)
			m_layout->RenderBackground(painter);
	}

	void Widget::RenderText(video::Painter* painter)
	{
		if (m_layout)
			m_layout->RenderText(painter);
	}

	void Widget::CollectBatches(GuiLayerBatch& batch)
	{
		if (m_layout)
			m_layout->CollectBatches(batch);
	}

	void Widget::SetRect(const math::Rect& rect)
	{
		m_rect = rect;
		UpdateLayout();
	}

	void Widget::SetPosition(s32 x, s32 y)
	{
		auto size = m_rect.GetSize();
		m_rect.Set(x, y, x + size.x, y + size.y);
		UpdateLayout();
	}

	void Widget::SetSize(const math::Vector2Int& size)
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

	bool Widget::IsInFocus() const 
	{ 
		return GuiManager::GetInstance()->GetFocusWidget().get() == this;
	}

	void Widget::UpdateLayout()
	{
		if (m_layout)
		{
			m_layout->Resize(m_rect);
		}
	}

} // gui

} // sh