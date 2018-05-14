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
    
    WidgetPtr Widget::FindChild(const String& name)
    {
        if (!m_layout)
            return WidgetPtr();
        
        const auto itemsCount = m_layout->GetItemsCount();
        for (size_t i = 0U; i < itemsCount; ++i)
        {
            const WidgetPtr& child = m_layout->GetWidget(i);
            if (child)
            {
                if (child->GetName() == name)
                {
                    return child;
                }
                else
                {
                    auto res = child->FindChild(name);
                    if (res)
                        return res;
                }
                
            }
        }
        
        return WidgetPtr();
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
			AdjustSize();
		}
	}

	void Widget::AdjustSize()
	{
		auto layoutRect = m_layout->GetRect();
		layoutRect.upperLeftCorner.x -= m_layout->GetLeftMargin();
		layoutRect.upperLeftCorner.y -= m_layout->GetTopMargin();
		layoutRect.lowerRightCorner.x += m_layout->GetRightMargin();
		layoutRect.lowerRightCorner.y += m_layout->GetBottomMargin();

		if (layoutRect.GetWidth() < m_rect.GetWidth())
			m_rect.lowerRightCorner.x = layoutRect.lowerRightCorner.x;

		if (layoutRect.GetHeight() < m_rect.GetHeight())
			m_rect.lowerRightCorner.y = layoutRect.lowerRightCorner.y;
	}

} // gui

} // sh
