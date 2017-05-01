#include "LineEdit.h"

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
	void TransformVec(const math::Matrix4f& mat, math::Vector4f& vec)
	{
		math::Vector4f temp;
		temp.x = (vec.x / 640.0f) * 2.0f - 1.0f;
		temp.y = ((480.0f - vec.y) / 480.0f) * 2.0f - 1.0f;
		vec = temp;
	}

	LineEdit::LineEdit(const math::Rectu& rect,
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

	void LineEdit::GetGeometry(GuiBatchData& data)
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

	void LineEdit::GetText(GuiBatchData& data)
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

	bool LineEdit::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		if (type == MouseEventType::ButtonPressed)
		{
			bool inside = m_rect.IsPointInside(x, y);
			if (inside)
			{
				if (m_inFocus)
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
				m_inFocus = !m_inFocus;

				return true;
			}
			else
			{
				if (m_inFocus)
				{
					UpdateUV(m_releasedSprite->GetUVRect().upperLeftCorner, 
						m_releasedSprite->GetUVRect().lowerRightCorner);
					UpdateColor(m_releasedSprite->GetColor());
					m_inFocus = false;
					return true;
				}
			}
		}
		
		return false;
	}

	bool LineEdit::ProcessKeyboardInput(KeyboardEventType type, KeyCode code)
	{
		if (m_inFocus && type == KeyboardEventType::KeyPressed)
		{
			if (code == KeyCode::KEY_BACK)
			{
				if (m_text.size() >= 1)
					m_text.pop_back();
			}
			else
				m_text += (char)code;
			return true;
		}
		return false;
	}

	void LineEdit::UpdatePosition()
	{
		const math::Matrix4f& ortho = sh::Device::GetInstance()->GetSceneManager()->GetCamera()->Get2DProjectionMatrix();
		math::Vector4f leftUp((float)m_rect.upperLeftCorner.x, (float)m_rect.upperLeftCorner.y, 0.0f, 1.0f);
		//leftUp = ortho * leftUp;
		TransformVec(ortho, leftUp);

		math::Vector4f rightDown((float)m_rect.lowerRightCorner.x, (float)m_rect.lowerRightCorner.y, 0.0f, 1.0f);
		//rightDown = ortho * rightDown;
		TransformVec(ortho, rightDown);

		m_batchData[0] = leftUp.x; m_batchData[1] = leftUp.y; m_batchData[2] = 0.0f;
		m_batchData[8] = leftUp.x; m_batchData[9] = rightDown.y; m_batchData[10] = 0.0f;
		m_batchData[16] = rightDown.x; m_batchData[17] = rightDown.y; m_batchData[18] = 0.0f;
		m_batchData[24] = rightDown.x; m_batchData[25] = leftUp.y; m_batchData[26] = 0.0f;
	}

	void LineEdit::UpdateUV(const math::Vector2f& uvLeftUp, const math::Vector2f& uvRightDown)
	{
		m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
		m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
		m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
		m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 
	}

	void LineEdit::UpdateColor(const math::Vector3f& color)
	{
		m_batchData[5] = color.x; m_batchData[6] = color.y; m_batchData[7] = color.z;
		m_batchData[13] = color.x; m_batchData[14] = color.y; m_batchData[15] = color.z; 
		m_batchData[21] = color.x; m_batchData[22] = color.y; m_batchData[23] = color.z; 
		m_batchData[29] = color.x; m_batchData[30] = color.y;  m_batchData[31] = color.z; 
	}
} // gui

} // sh