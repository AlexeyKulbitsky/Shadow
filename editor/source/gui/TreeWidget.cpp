#include "TreeWidget.h"

TreeExpandButton::TreeExpandButton()
{
	const auto& ref = sh::gui::GuiManager::GetInstance()->GetStyle()->GetButton("TreeButton");

	m_sprites[Released] = ref->GetSprite(Released);
	m_sprites[Pressed] = ref->GetSprite(Pressed);
	m_sprites[Hovered] = ref->GetSprite(Hovered);

	m_toggleable = true;
	SetMaximumWidth(20);

	m_rect = sh::math::Recti(0, 0, 10, 10);
}

TreeExpandButton::~TreeExpandButton()
{
}

void TreeExpandButton::Render(sh::video::Painter* painter)
{
	if (!m_visible)
		return;

	painter->SetMaterial(sh::gui::GuiManager::GetInstance()->GetDefaultMaterial());
	sh::math::Vector2f upperUVLeft, lowerUVRight;
	sh::math::Vector4f color(1.0f);
	if (m_toggled)
	{
		upperUVLeft = m_sprites[Pressed]->GetUVRect().upperLeftCorner;
		lowerUVRight = m_sprites[Pressed]->GetUVRect().lowerRightCorner;
	}
	else
	{
		upperUVLeft = m_sprites[Released]->GetUVRect().upperLeftCorner;
		lowerUVRight = m_sprites[Released]->GetUVRect().lowerRightCorner;
	}

	if (m_state == Hovered)
		color = sh::math::Vector4f(0.5f, 0.5f, 0.5f, 1.0f);

	sh::video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, upperUVLeft, color);
	sh::video::Painter::Vertex downRight(m_rect.lowerRightCorner, lowerUVRight, color);
		
	painter->DrawRect(upperLeft, downRight);
}

TreeItem::TreeItem(const sh::String& name, TreeItem* parent)
	: sh::gui::Button()
	, m_parent(parent)
{  
	sh::gui::LabelPtr label(new sh::gui::Label(name));
	SetToggleable(true);

	OnToggle.Connect(std::bind(&TreeItem::OnToggled, this, std::placeholders::_1));
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	layout->AddWidget(label);
	SetMinimumHeight(15U);
	SetMaximumHeight(20U);
	SetLayout(layout);

	if (parent)
	{
		sh::s32 offset = parent->GetOffset();
		m_offset = offset + 20U;
		layout->SetMargins(0, 0, 0, m_offset);
	}
}

TreeItem::~TreeItem() 
{ 
}

void TreeItem::AddChild(const sh::SPtr<TreeItem>& child)
{
	m_children.push_back(child);
}

void TreeItem::SetExpanded(bool expanded)
{ 
	m_expanded = expanded; 
}

void TreeItem::OnToggled(bool toggled)
{
}

bool TreeItem::ProcessEvent(sh::gui::GUIEvent& ev)
{
	if (ev.type == sh::gui::EventType::PointerDown &&
		ev.mouseButtonCode == sh::MouseCode::ButtonRight)
		return false;

	if (ev.type == sh::gui::EventType::PointerUp &&
		ev.mouseButtonCode == sh::MouseCode::ButtonRight &&
		m_rect.IsPointInside(ev.x, ev.y))
	{
		OnContextMenu(ev.x, ev.y);
		return true;
	}
	if (ev.type == sh::gui::EventType::PointerDoubleClick && 
		ev.mouseButtonCode == sh::MouseCode::ButtonLeft &&
		m_rect.IsPointInside(ev.x, ev.y))
	{
		OnEdit(ev.x, ev.y);
		return true;
	}
	else
	{
		return Widget::ProcessEvent(ev);
	}
}

//////////////////////////////////////////////////////////////////////////////////

TreeWidget::TreeWidget()
{
	m_layout.reset(new sh::gui::VerticalLayout());
	m_layout->SetSpacing(1);
	SetLayout(m_layout);
}

void TreeWidget::AddItem(sh::SPtr<TreeItem> item)
{
	sh::u32 indexToInsert = 0U;
	TreeItem* parent = item->GetParent();
	if (parent)
	{
		struct Local
		{
			static TreeItem* GetLastItemToIsert(TreeItem* parent)
			{
				if (parent->m_children.size() == 0)
					return parent;

				const size_t lastChildIndex = parent->m_children.size() - 1;
				return GetLastItemToIsert(parent->m_children[lastChildIndex].get());
			}
		};

		auto lastChildToInsert = Local::GetLastItemToIsert(parent);
		auto itemsCount = m_layout->GetItemsCount();
		for (sh::u32 i = 0U; i < itemsCount; ++i)
		{
			auto treeItem = static_cast<TreeItem*>(m_layout->GetWidget(i).get());
			if (treeItem == lastChildToInsert)
			{
				indexToInsert = i + 1;
				break;
			}
		}
		parent->m_children.push_back(item);
	}

	item->m_treeWidget = this;
	m_layout->InsertWidget(indexToInsert, item);
}

void TreeWidget::Render(sh::video::Painter* painter)
{
	painter->SetClipRect(sh::math::Rectu(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y,
		m_rect.lowerRightCorner.x, m_rect.lowerRightCorner.y));
	
	if (m_layout)
	{
		const sh::u32 itemsCount = m_layout->GetItemsCount();
		for (sh::u32 i = 0U; i < itemsCount; ++i)
		{
			auto treeItem = std::static_pointer_cast<TreeItem>(m_layout->GetWidget(i));
			if (treeItem->IsVisible() && m_rect.Intersects(treeItem->GetRect()))
				treeItem->Render(painter);
		}
	}

	painter->SetClipRect(sh::Device::GetInstance()->GetDriver()->GetViewport());

	RenderScrollBars(painter);
}

void TreeWidget::UpdateLayout()
{
	if (m_layout)
	{
		const sh::u32 itemsCount = m_layout->GetItemsCount();
		sh::u32 height = 0U;
		auto startPos = m_rect.upperLeftCorner;

		for (sh::u32 i = 0U; i < itemsCount; ++i)
		{
			auto treeItem = std::static_pointer_cast<TreeItem>(m_layout->GetWidget(i));

			TreeItem* parent = treeItem->GetParent();
			bool expanded = true;
			while(parent)
			{
				if (!parent->IsExpanded())
				{
					expanded = false;
					break;
				}
				parent = parent->GetParent();
			}

			if (!expanded)
			{
				treeItem->SetVisible(false);
				treeItem->SetMaximumHeight(0U);
				treeItem->SetHeight(0U);
				continue;
			}
			else
			{
				treeItem->SetVisible(true);
				treeItem->SetMaximumHeight(15U);
				treeItem->SetHeight(15U);
			}

			treeItem->SetPosition(startPos.x, startPos.y + height);
			height += m_layout->GetItem(i)->GetHeight() + m_layout->GetSpacing();
		}
		auto r = m_rect;
		r.lowerRightCorner.y = r.upperLeftCorner.y + height;


		sh::s32 delta = m_fullRect.upperLeftCorner.y - r.upperLeftCorner.y;
		r.lowerRightCorner.y += delta;
		r.upperLeftCorner.y += delta;
		if (r.lowerRightCorner.y < m_rect.lowerRightCorner.y)
		{
			sh::s32 delta = m_rect.lowerRightCorner.y - r.lowerRightCorner.y;
			r.lowerRightCorner.y += delta;
			r.upperLeftCorner.y += delta;
		}
		if (r.upperLeftCorner.y > m_rect.upperLeftCorner.y)
		{
			sh::s32 delta = m_rect.upperLeftCorner.y - r.upperLeftCorner.y;
			r.lowerRightCorner.y += delta;
			r.upperLeftCorner.y += delta;
		}

		m_layout->Resize(r);
		m_fullRect = r;

		m_scrollSpeed = height / 25;
	}
}

bool TreeWidget::ProcessEvent(sh::gui::GUIEvent& ev)
{
	return sh::gui::ScrollWidget::ProcessEvent(ev);
}