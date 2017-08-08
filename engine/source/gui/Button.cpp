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




namespace sh
{

namespace gui
{
	Button::Button()
		: Widget()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetButton();

		m_releasedSprite = ref->m_releasedSprite;
		m_pressedSprite = ref->m_pressedSprite;
		m_hoveredSprite = ref->m_hoveredSprite;

		m_rect = sh::math::Rectu(0U, 0U, 10U, 10U);
		m_text.reset(new Text(m_rect));

		UpdatePosition();
		UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner, m_releasedSprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_releasedSprite->GetColor());
	}

	Button::Button(const math::Rectu rect)
		: Widget()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetButton();

		m_releasedSprite = ref->m_releasedSprite;
		m_pressedSprite = ref->m_pressedSprite;
		m_hoveredSprite = ref->m_hoveredSprite;

		m_rect = rect;
		m_text.reset(new Text(rect));

		UpdatePosition();
		UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner, m_releasedSprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_releasedSprite->GetColor());
	}

	Button::Button(const SpritePtr& defaultSprite,
		const SpritePtr& pressedSprite,
		const SpritePtr& hoveredSprite)
		: Widget()
		, m_releasedSprite(defaultSprite)
		, m_pressedSprite(pressedSprite)
		, m_hoveredSprite(hoveredSprite)
	{
		m_rect = sh::math::Rectu(0U, 0U, 10U, 10U);
		m_text.reset(new Text(m_rect));
	}

	Button::Button(const math::Rectu& rect,
				   const SpritePtr& defaultSprite,
				   const SpritePtr& pressedSprite,
				   const SpritePtr& hoveredSprite)
				   : Button(defaultSprite, pressedSprite, hoveredSprite)
	{
		m_rect = rect;
		m_text.reset(new Text(rect));

		UpdatePosition();
		UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner, m_releasedSprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_releasedSprite->GetColor());
	}

	Button::Button(const String& text)
		: Widget()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetButton();

		m_releasedSprite = ref->m_releasedSprite;
		m_pressedSprite = ref->m_pressedSprite;
		m_hoveredSprite = ref->m_hoveredSprite;

		m_rect = sh::math::Rectu(0U, 0U, 10U, 10U);
		m_text.reset(new Text(m_rect));
		m_text->SetText(text);

		UpdatePosition();
		UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner, m_releasedSprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_releasedSprite->GetColor());
	}

	ButtonPtr Button::Clone() const
	{
		ButtonPtr result(new Button());
		result->m_rect = m_rect;

		result->m_releasedSprite = m_releasedSprite;
		result->m_pressedSprite = m_pressedSprite;
		result->m_hoveredSprite = m_hoveredSprite;

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
			UpdateUV(m_pressedSprite->GetUVRect().upperLeftCorner,
				m_pressedSprite->GetUVRect().lowerRightCorner);
			UpdateColor(m_pressedSprite->GetColor());
		}
		else
		{
			UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner,
				m_releasedSprite->GetUVRect().lowerRightCorner);
			UpdateColor(m_releasedSprite->GetColor());
		}

		OnToggle(m_toggled, std::static_pointer_cast<Button>(shared_from_this()));
	}

	void Button::GetGeometry(GuiBatchData& data)
	{
		/*GuiElement::GetGeometry(data);*/
		Widget::GetGeometry(data);
	}

	void Button::GetTextGeometry(GuiBatchData& data)
	{
		m_text->GetTextGeometry(data);
	}

	void Button::SetPosition(u32 x, u32 y)
	{
		/*GuiElement::SetPosition(x, y);*/
		Widget::SetPosition(x, y);
		m_text->SetPosition(x, y);
	}

	void Button::SetSize(const math::Vector2u& size)
	{
		/*GuiElement::SetSize(size);*/
		Widget::SetSize(size);
		m_text->SetSize(size);
	}

	void Button::SetWidth(u32 width)
	{
		/*GuiElement::SetWidth(width);*/
		Widget::SetWidth(width);
		m_text->SetWidth(width);
	}

	void Button::SetHeight(u32 height)
	{
		/*GuiElement::SetHeight(height);*/
		Widget::SetHeight(height);
		m_text->SetHeight(height);
	}

	bool Button::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		bool inside = m_rect.IsPointInside(x, y);
		if (inside)
		{
			switch (type)
			{
				case MouseEventType::ButtonPressed:
				{
					if (m_toggleable)
					{
						m_toggled = !m_toggled;

						if (m_toggled)
						{
							UpdateUV(m_pressedSprite->GetUVRect().upperLeftCorner,
								m_pressedSprite->GetUVRect().lowerRightCorner);
							UpdateColor(m_pressedSprite->GetColor());
						}
						else
						{
							UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner,
								m_releasedSprite->GetUVRect().lowerRightCorner);
							UpdateColor(m_releasedSprite->GetColor());
						}

						OnToggle(m_toggled, std::static_pointer_cast<Button>(shared_from_this()));
					}
					else
					{
						UpdateUV(m_pressedSprite->GetUVRect().upperLeftCorner, 
								 m_pressedSprite->GetUVRect().lowerRightCorner);
						UpdateColor(m_pressedSprite->GetColor());
					}

					OnPress(std::static_pointer_cast<Button>(shared_from_this()));

					return true;
				}
					break;
				case MouseEventType::ButtonReleased:
				{
					if (!m_toggleable)
					{
						UpdateUV(m_hoveredSprite->GetUVRect().upperLeftCorner, 
								 m_hoveredSprite->GetUVRect().lowerRightCorner);
						UpdateColor(m_hoveredSprite->GetColor());
					}

					OnRelease(std::static_pointer_cast<Button>(shared_from_this()));

					return true;
				}
					break;
				case MouseEventType::Moved:
				{
					if (!m_toggleable || !m_toggled)
					{
						UpdateUV(m_hoveredSprite->GetUVRect().upperLeftCorner,
							m_hoveredSprite->GetUVRect().lowerRightCorner);
						UpdateColor(m_hoveredSprite->GetColor());
					}

					OnHover(std::static_pointer_cast<Button>(shared_from_this()));

					return false;
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
				UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner,
					m_releasedSprite->GetUVRect().lowerRightCorner);
				UpdateColor(m_releasedSprite->GetColor());
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

	const math::Vector2u& Button::GetPosition() const
	{
		return m_rect.upperLeftCorner;
	}


} // gui

} // sh