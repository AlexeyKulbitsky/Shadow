#include "Button.h"

#include "GuiManager.h"
#include "Sprite.h"
#include "Text.h"
#include "Style.h"

#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "../video/VertexDeclaration.h"
#include "../video/Driver.h"

#include "../font/Font.h"
#include "../Device.h"

#include "../serialization/ObjectFactory.h"

#include <pugixml.hpp>

#include <iostream>
namespace sh
{

namespace gui
{
	Button::Button()
		: Widget()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetButton();

		m_sprites[Released] = ref->m_sprites[Released];
		m_sprites[Pressed] = ref->m_sprites[Pressed];
		m_sprites[Hovered] = ref->m_sprites[Hovered];

		m_rect = sh::math::Rect(0, 0, 10, 10);
		m_text.reset(new Text(m_rect));
        m_name = "Button";
	}

	Button::Button(const math::Rect rect)
		: Widget()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetButton();

		m_sprites[Released] = ref->m_sprites[Released];
		m_sprites[Pressed] = ref->m_sprites[Pressed];
		m_sprites[Hovered] = ref->m_sprites[Hovered];

		m_rect = rect;
		m_text.reset(new Text(rect));
        m_name = "Button";
	}

	Button::Button(const SpritePtr& defaultSprite,
		const SpritePtr& pressedSprite,
		const SpritePtr& hoveredSprite)
		: Widget()
	{
		m_sprites[Released] = defaultSprite;
		m_sprites[Pressed] = pressedSprite;
		m_sprites[Hovered] = hoveredSprite;

		m_rect = sh::math::Rect(0, 0, 10, 10);
		m_text.reset(new Text(m_rect));
        m_name = "Button";
	}

	Button::Button(const math::Rect& rect,
				   const SpritePtr& defaultSprite,
				   const SpritePtr& pressedSprite,
				   const SpritePtr& hoveredSprite)
				   : Button(defaultSprite, pressedSprite, hoveredSprite)
	{
		m_rect = rect;
		m_text.reset(new Text(rect));
        m_name = "Button";
	}

	Button::Button(const String& text)
		: Widget()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetButton();

		m_sprites[Released] = ref->m_sprites[Released];
		m_sprites[Pressed] = ref->m_sprites[Pressed];
		m_sprites[Hovered] = ref->m_sprites[Hovered];

		m_rect = sh::math::Rect(0, 0, 10, 10);
		m_text.reset(new Text(m_rect));
		m_text->SetText(text);
        m_name = "Button";
	}
    
    void Button::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<Button>("UI");
        ObjectFactory::GetInstance()->RegisterParentProperties<Button, Widget>();
        S_ACCESSOR_PROPERTY("Text", GetText, SetText);
        S_ACCESSOR_PROPERTY("Toggleable", IsToggleable, SetToggleable);
        
        
    }

	ButtonPtr Button::Clone() const
	{
		ButtonPtr result(new Button(m_sprites[Released], m_sprites[Pressed], m_sprites[Hovered]));
		result->m_rect = m_rect;

		result->m_minSize = m_minSize;
		result->m_maxSize = m_maxSize;
		result->m_toggleable = m_toggleable;

		return result;
	}

	void Button::Load(const pugi::xml_node& node)
	{
		pugi::xml_node rect = node.child("rect");
		SH_ASSERT(rect, "Can not find Rect node for button!");
		u32 x1 = rect.attribute("x1").as_uint();
		u32 y1 = rect.attribute("y1").as_uint();
		u32 x2 = rect.attribute("x2").as_uint();
		u32 y2 = rect.attribute("y2").as_uint();
		m_rect.Set(x1, y1, x2, y2);
	}

	void Button::SetText( const String& text )
	{
		m_text->SetText(text);
	}

	const String& Button::GetText() const
	{
		return m_text->GetText();
	}

	void Button::SetToggled(bool toggled)
	{
		if (!m_toggleable)
			return;

		m_toggled = toggled;

		if (m_toggled)
		{
			m_state = Pressed;
		}
		else
		{
			m_state = Released;
		}

		OnToggle(m_toggled, std::static_pointer_cast<Button>(shared_from_this()));
	}

	void Button::Render(video::Painter* painter)
	{
		if (!m_visible)
			return;

		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, 
										 m_sprites[m_state]->GetUpperLeftUV(), 
										 m_sprites[m_state]->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
										 m_sprites[m_state]->GetLowerRightUV(),
										 m_sprites[m_state]->GetColor());
		painter->DrawRect(upperLeft, downRight);

		//Widget::Render(painter);

		m_text->Render(painter);
	}

	void Button::CollectBatches(GuiLayerBatch& batch)
	{
		if (!m_visible)
			return;

		auto mat = GuiManager::GetInstance()->GetDefaultMaterial().get();
		auto& backgroundLayer = batch.batches[GUI_LAYER_BACKGROUND];

		if (backgroundLayer.batches[backgroundLayer.batches.size() - 1].material != mat)
		{
			const auto& prevBatch = backgroundLayer.batches[backgroundLayer.batches.size() - 1];
			GuiBatch newBatch;
			newBatch.clipRect = prevBatch.clipRect;
			newBatch.material = mat;
			newBatch.indicesCount = 0;
			newBatch.startIndex = batch.batches[GUI_LAYER_BACKGROUND].indexArray.size();

			backgroundLayer.batches.push_back(newBatch);
		}

		auto& subBatch = backgroundLayer.batches[backgroundLayer.batches.size() - 1];
		subBatch.indicesCount += 6;

		backgroundLayer.AddRect(m_rect.upperLeftCorner, m_sprites[m_state]->GetUpperLeftUV(), m_sprites[m_state]->GetColor(),
			m_rect.lowerRightCorner, m_sprites[m_state]->GetLowerRightUV(), m_sprites[m_state]->GetColor());

	}

	void Button::RenderBackground(video::Painter* painter)
	{
		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner,
			m_sprites[m_state]->GetUpperLeftUV(),
			m_sprites[m_state]->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
			m_sprites[m_state]->GetLowerRightUV(),
			m_sprites[m_state]->GetColor());
		painter->DrawRect(upperLeft, downRight);
        
        
	}

	void Button::RenderText(video::Painter* painter)
	{
		m_text->Render(painter);
	}

	void Button::SetRect(const math::Rect& rect)
	{
		Widget::SetRect(rect);
		m_text->SetRect(rect);
	}

	void Button::SetPosition(s32 x, s32 y)
	{
		Widget::SetPosition(x, y);
		m_text->SetPosition(x, y);
	}

	void Button::SetSize(const math::Vector2Int& size)
	{
		Widget::SetSize(size);
		m_text->SetSize(size);
	}

	void Button::SetWidth(s32 width)
	{
		Widget::SetWidth(width);
		m_text->SetWidth(width);
	}

	void Button::SetHeight(s32 height)
	{
		Widget::SetHeight(height);
		m_text->SetHeight(height);
	}

	bool Button::ProcessEvent(GUIEvent& ev)
	{
		bool inside = m_rect.IsPointInside(ev.x, ev.y);
		if (inside)
		{
			switch (ev.type)
			{
			case EventType::PointerDown:
			{
				if (m_toggleable)
				{
					m_toggled = !m_toggled;

					if (m_toggled)
					{
						m_state = Pressed;
					}
					else
					{
						m_state = Hovered;
						OnHover(std::static_pointer_cast<Button>(shared_from_this()));
					}

					OnToggle(m_toggled, std::static_pointer_cast<Button>(shared_from_this()));
				}
				else
				{
					m_state = Pressed;
				}

				OnPress(std::static_pointer_cast<Button>(shared_from_this()));

				return true;
			}
			break;
			case EventType::PointerUp:
			{
				if (!m_toggleable || !m_toggled)
				{
					m_state = Hovered;
				}

				OnRelease(std::static_pointer_cast<Button>(shared_from_this()));

				return true;
			}
			break;
			case EventType::PointerMove:
			{
				if (!m_toggleable || !m_toggled)
				{
					m_state = Hovered;
				}
                
				OnHover(std::static_pointer_cast<Button>(shared_from_this()));

				return false;
			}
			break;
            case EventType::PointerDoubleClick:
            {
                OnDoubleClick(std::static_pointer_cast<Button>(shared_from_this()));
            }
            break;
			default:
				break;
			}
		}
		else
		{
			if (!m_toggleable || !m_toggled)
			{
				m_state = Released;
			}
		}

		return false;
	}

	u32 Button::GetHeight() const
	{
		return m_rect.GetHeight();
	}

	u32 Button::GetWidth() const
	{
		return m_rect.GetWidth();
	}

	const math::Vector2Int& Button::GetPosition() const
	{
		return m_rect.upperLeftCorner;
	}

	SpritePtr Button::GetSprite(State state) const
	{
		if (state >= State::Count)
			return SpritePtr();

		return m_sprites[static_cast<size_t>(state)];
	}


} // gui

} // sh
