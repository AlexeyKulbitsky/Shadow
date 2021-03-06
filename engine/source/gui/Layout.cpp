#include "Layout.h"
#include "../serialization/Serializer.h"
#include "../serialization/ObjectFactory.h"

namespace sh
{

namespace gui
{
    void WidgetItem::Serialize(Serializer* serializer)
    {
        if (m_widget)
            m_widget->Serialize(serializer);
    }
    
    void WidgetItem::Deserialize(Serializer* serializer)
    {
        if (m_widget)
            m_widget->Deserialize(serializer);
    }
    
	void WidgetItem::Resize(const math::Rect& rect)
	{
		//m_widget->SetPosition(rect.upperLeftCorner.x, rect.upperLeftCorner.y);
		s32 width = rect.GetWidth();
		s32 height = rect.GetHeight();
		
		if (width < m_widget->GetMinimumWidth())
			width = m_widget->GetMinimumWidth();
		if (width > m_widget->GetMaximumWidth())
			width = m_widget->GetMaximumWidth();

		if (height < m_widget->GetMinimumHeight())
			height = m_widget->GetMinimumHeight();
		if (height > m_widget->GetMaximumHeight())
			height = m_widget->GetMaximumHeight();

		const math::Rect finalRect(rect.upperLeftCorner.x, rect.upperLeftCorner.y,
									rect.upperLeftCorner.x + width, rect.upperLeftCorner.y + height);
		//m_widget->SetWidth(width);
		//m_widget->SetHeight(height);
		m_widget->SetRect(finalRect);
	}

	////////////////////////////////////////////////////////////////////
    
    void Layout::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<Layout>("UI");
        S_ACCESSOR_PROPERTY("Spacing", GetSpacing, SetSpacing);
        S_ACCESSOR_PROPERTY("TopMargin", GetTopMargin, SetTopMargin);
        S_ACCESSOR_PROPERTY("BottomMargin", GetBottomMargin, SetBottomMargin);
        S_ACCESSOR_PROPERTY("RightMargin", GetRightMargin, SetRightMargin);
        S_ACCESSOR_PROPERTY("LeftMargin", GetLeftMargin, SetLeftMargin);
    }
    
    void Layout::Serialize(Serializer* serializer)
    {
        Serializer* childSerializer = serializer->Child();
        childSerializer->Serialize(this);
        for (size_t i = 0U; i < m_items.size(); ++i)
        {
            m_items[i]->Serialize(childSerializer);
        }
        delete childSerializer;
    }
    
    void Layout::Deserialize(Serializer* serializer)
    {
        serializer->Deserialize(this);
        for (size_t i = 0U; i < m_items.size(); ++i)
        {
            m_items[i]->Deserialize(serializer);
        }
    }

	void Layout::AddWidget(const WidgetPtr& widget)
	{
		LayoutItemPtr item(new WidgetItem(widget));
		m_items.push_back(item);

		if (m_parent)
			m_parent->UpdateLayout();
	}

	void Layout::InsertWidget(u32 index, const WidgetPtr& widget)
	{
		LayoutItemPtr item(new WidgetItem(widget));
		m_items.insert(m_items.begin() + index, item);

		if (m_parent)
			m_parent->UpdateLayout();
	}

	void Layout::RemoveWidget(const WidgetPtr& widget)
	{
		for (auto item = m_items.begin(); item != m_items.end(); ++item)
		{
			if ((*item)->GetWidget() == widget)
			{
				m_items.erase(item);
				return;
			}
		}
	}

	void Layout::Clear()
	{
		m_items.clear();
	}

	void Layout::AddLayout(const LayoutPtr& layout)
	{
		m_items.push_back(layout);

		if (m_parent)
			m_parent->UpdateLayout();
	}

	void Layout::SetSize(const math::Vector2Int& size)
	{
		
	}

	void Layout::SetWidth(s32 width)
	{

	}

	void Layout::SetHeight(s32 height)
	{

	}

	void Layout::SetMargins(u32 top, u32 right, u32 bottom, u32 left)
	{
		m_topMargin = top;
		m_rightMargin = right;
		m_bottomMargin = bottom;
		m_leftMargin = left;
	}
    
    void Layout::SetTopMargin(u32 margin)
    {
        m_topMargin = margin;
    }
    
    void Layout::SetBottomMargin(u32 margin)
    {
        m_bottomMargin = margin;
    }
    
    void Layout::SetRightMargin(u32 margin)
    {
        m_rightMargin = margin;
    }
    
    void Layout::SetLeftMargin(u32 margin)
    {
        m_leftMargin = margin;
    }
    
    const u32 Layout::GetTopMargin() const
    {
        return m_topMargin;
    }
    
    const u32 Layout::GetBottomMargin() const
    {
        return m_bottomMargin;
        
    }
    
    const u32 Layout::GetRightMargin() const
    {
        return m_rightMargin;
        
    }
    
    const u32 Layout::GetLeftMargin() const
    {
        return m_leftMargin;
        
    }

	void Layout::Render(video::Painter* painter)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget)
			{
				if (widget->IsVisible() && !widget->IsInFocus())
					widget->Render(painter);
				continue;
			}

			m_items[i]->Render(painter);
		}
	}

	void Layout::CollectBatches(GuiLayerBatch& batch)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget && widget->IsVisible() && !widget->IsInFocus())
			{
				widget->CollectBatches(batch);
				continue;
			}

			m_items[i]->CollectBatches(batch);
		}
	}

	void Layout::RenderBackground(video::Painter* painter)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget && widget->IsVisible() && !widget->IsInFocus())
			{
				widget->RenderBackground(painter);
				continue;
			}

			m_items[i]->RenderBackground(painter);
		}
	}

	void Layout::RenderText(video::Painter* painter)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget && widget->IsVisible() && !widget->IsInFocus())
			{
				widget->RenderText(painter);
				continue;
			}

			m_items[i]->RenderText(painter);
		}
	}

	bool Layout::ProcessEvent(GUIEvent& ev)
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			const auto& widget = m_items[i]->GetWidget();
			if (widget && !widget->IsInFocus())
			{
				if (widget->ProcessEvent(ev))
					return true;
			}

			if (m_items[i]->ProcessEvent(ev))
				return true;
		}

		return false;
	}


} // gui

} // sh
