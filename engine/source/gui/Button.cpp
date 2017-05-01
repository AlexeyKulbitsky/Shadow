#include "Button.h"

#include "GuiManager.h"
#include "Sprite.h"

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
	void TransformVector(const math::Matrix4f& mat, math::Vector4f& vec)
	{
		math::Vector4f temp;
		temp.x = (vec.x / 640.0f) * 2.0f - 1.0f;
		temp.y = ((480.0f - vec.y) / 480.0f) * 2.0f - 1.0f;
		vec = temp;
	}

	Button::Button()
	{
		m_batchData.resize(4 * 8);
	}

	Button::Button(const math::Rectu& rect,
				   const SpritePtr& defaultSprite,
				   const SpritePtr& pressedSprite,
				   const SpritePtr& hoveredSprite)
	{
		m_batchData.resize(4 * 8);
		m_rect = rect;
		m_releasedSprite = defaultSprite;
		m_pressedSprite = pressedSprite;
		m_hoveredSprite = hoveredSprite;

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

	void Button::GetGeometry(GuiBatchData& data)
	{
		data.vertices.insert(data.vertices.end(), m_batchData.begin(), m_batchData.end());

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 1);
		data.indices.push_back(data.verticesCount + 2);

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 2);
		data.indices.push_back(data.verticesCount + 3);
		data.verticesCount += 4;
	}

	void Button::GetTextGeometry(GuiBatchData& data)
	{
		const auto& font = GuiManager::GetInstance()->GetFont();
		s32 width = font->GetTextureAtlas()->GetDescription().width; 
		s32 height = font->GetTextureAtlas()->GetDescription().height;

		s32 xOrigin = m_rect.upperLeftCorner.x + 5;
		s32 yOrigin = m_rect.lowerRightCorner.y - m_rect.GetHeight() / 4;

		for (const auto c : m_text)
		{
			const auto& desc = font->GetGlyphDescription(static_cast<u32>(c));

			float x1 = 2.0f * (float)(xOrigin + desc.x_off) / 640.0f - 1.0f;
			float y1 = 2.0f * (480.0f - (float)(yOrigin - desc.y_off)) / 480.0f - 1.0f;
			float x2 = 2.0f * (float)(xOrigin + desc.x_off + desc.width) / 640.0f - 1.0f;
			float y2 = 2.0f * (480.0f - (float)(yOrigin - desc.y_off + desc.height)) / 480.0f - 1.0f;

			float u1 = (float)desc.x0 / (float)width;
			float v1 = (float)desc.y0 / (float)height;
			float u2 = (float)desc.x1 / (float)width;
			float v2 = (float)desc.y1 / (float)height;

			data.vertices.insert(data.vertices.end(), {x1, y1, 0.0f, u1, v1});
			data.vertices.insert(data.vertices.end(), {x1, y2, 0.0f, u1, v2});
			data.vertices.insert(data.vertices.end(), {x2, y2, 0.0f, u2, v2});
			data.vertices.insert(data.vertices.end(), {x2, y1, 0.0f, u2, v1});

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 1);
			data.indices.push_back(data.verticesCount + 2);

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 2);
			data.indices.push_back(data.verticesCount + 3);
			data.verticesCount += 4;

			xOrigin += desc.advance;
		}
	}

	void Button::SetPosition(u32 x, u32 y)
	{
		auto size = m_rect.GetSize();
		m_rect.Set(x, y, x + size.x, y + size.y);

		UpdatePosition();
	}

	void Button::SetSize(const math::Vector2u& size)
	{
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + size.y);

		UpdatePosition();
	}

	void Button::SetWidth(u32 width)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + width, pos.y + size.y);

		UpdatePosition();
	}

	void Button::SetHeight(u32 height)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + height);

		UpdatePosition();
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
						if (m_toggled)
						{
							UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner, 
									 m_releasedSprite->GetUVRect().lowerRightCorner);
							UpdateColor(m_releasedSprite->GetColor());
						}
						else
						{
							UpdateUV(m_pressedSprite->GetUVRect().upperLeftCorner, 
									 m_pressedSprite->GetUVRect().lowerRightCorner);
							UpdateColor(m_pressedSprite->GetColor());
						}

						m_toggled = !m_toggled;

						OnToggle(m_toggled);
					}
					else
					{
						UpdateUV(m_pressedSprite->GetUVRect().upperLeftCorner, 
								 m_pressedSprite->GetUVRect().lowerRightCorner);
						UpdateColor(m_pressedSprite->GetColor());
					}

					OnPress();

					return true;
				}
					break;
				case MouseEventType::ButtonReleased:
				{
					if (!m_toggleable)
					{
						UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner, 
								 m_releasedSprite->GetUVRect().lowerRightCorner);
						UpdateColor(m_releasedSprite->GetColor());
					}

					OnRelease();

					return true;
				}
					break;
				case MouseEventType::Moved:
				{
					UpdateUV(m_hoveredSprite->GetUVRect().upperLeftCorner, 
							 m_hoveredSprite->GetUVRect().lowerRightCorner);
					UpdateColor(m_hoveredSprite->GetColor());

					OnHover();

					return false;
				}
				break;
				default:
					break;
			}
		}
		else
		{
			UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner, 
					 m_releasedSprite->GetUVRect().lowerRightCorner);
			UpdateColor(m_releasedSprite->GetColor());
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

	void Button::UpdateText()
	{
		const auto& font = GuiManager::GetInstance()->GetFont();
		s32 width = font->GetTextureAtlas()->GetDescription().width; 
		s32 height = font->GetTextureAtlas()->GetDescription().height;

		s32 xOrigin = m_rect.upperLeftCorner.x + 5;
		s32 yOrigin = m_rect.lowerRightCorner.y - m_rect.GetHeight() / 4;

		m_textBatchData.clear();

		for (const auto c : m_text)
		{
			const auto& desc = font->GetGlyphDescription(static_cast<u32>(c));

			float x1 = 2.0f * (float)(xOrigin + desc.x_off) / 640.0f - 1.0f;
			float y1 = 2.0f * (480.0f - (float)(yOrigin - desc.y_off)) / 480.0f - 1.0f;
			float x2 = 2.0f * (float)(xOrigin + desc.x_off + desc.width) / 640.0f - 1.0f;
			float y2 = 2.0f * (480.0f - (float)(yOrigin - desc.y_off + desc.height)) / 480.0f - 1.0f;

			float u1 = (float)desc.x0 / (float)width;
			float v1 = (float)desc.y0 / (float)height;
			float u2 = (float)desc.x1 / (float)width;
			float v2 = (float)desc.y1 / (float)height;

			m_textBatchData.insert(m_textBatchData.end(), {x1, y1, 0.0f, u1, v1});
			m_textBatchData.insert(m_textBatchData.end(), {x1, y2, 0.0f, u1, v2});
			m_textBatchData.insert(m_textBatchData.end(), {x2, y2, 0.0f, u2, v2});
			m_textBatchData.insert(m_textBatchData.end(), {x2, y1, 0.0f, u2, v1});

			/*
			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 1);
			data.indices.push_back(data.verticesCount + 2);

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 2);
			data.indices.push_back(data.verticesCount + 3);
			data.verticesCount += 4;
			*/
			xOrigin += desc.advance;
		}
	}

} // gui

} // sh