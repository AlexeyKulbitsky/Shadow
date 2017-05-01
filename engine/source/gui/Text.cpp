#include "Text.h"

#include "GuiManager.h"
#include "../font/Font.h"
#include "../video/Texture.h"
#include "../Device.h"

namespace sh
{

namespace gui
{

	Text::Text()
	{

	}

	Text::Text(const math::Rectu rect)
	{
		m_rect = rect;
	}

	Text::Text(const String& text)
	{
		SetText(text);
	}

	Text::Text(const math::Rectu rect, const String& text)
	{
		m_rect = rect;
		SetText(text);
	}

	Text::~Text()
	{

	}

	void Text::SetText(const String& text)
	{
		m_text = text;
		UpdateTextGeometry();
	}

	void Text::GetTextGeometry(GuiBatchData& data)
	{
		data.vertices.insert(data.vertices.end(), m_textBatchData.begin(), m_textBatchData.end());

		for (u32 i = 0, sz = m_text.size(); i < sz; ++i)
		{
			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 1);
			data.indices.push_back(data.verticesCount + 2);

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 2);
			data.indices.push_back(data.verticesCount + 3);
			data.verticesCount += 4;
		}
	}

	void Text::UpdateTextGeometry()
	{
		const auto& viewPort = sh::Device::GetInstance()->GetDriver()->GetViewPort();

		const auto& font = GuiManager::GetInstance()->GetFont();
		s32 width = font->GetTextureAtlas()->GetDescription().width; 
		s32 height = font->GetTextureAtlas()->GetDescription().height;

		s32 xOrigin = m_rect.upperLeftCorner.x + 5;
		s32 yOrigin = m_rect.lowerRightCorner.y - m_rect.GetHeight() / 4;

		for (const auto c : m_text)
		{
			const auto& desc = font->GetGlyphDescription(static_cast<u32>(c));

			float x1 = 2.0f * (float)(xOrigin + desc.x_off) / viewPort.z - 1.0f;
			float y1 = 2.0f * (viewPort.w - (float)(yOrigin - desc.y_off)) / viewPort.w - 1.0f;
			float x2 = 2.0f * (float)(xOrigin + desc.x_off + desc.width) / viewPort.z - 1.0f;
			float y2 = 2.0f * (viewPort.w - (float)(yOrigin - desc.y_off + desc.height)) / viewPort.w - 1.0f;

			float u1 = (float)desc.x0 / (float)width;
			float v1 = (float)desc.y0 / (float)height;
			float u2 = (float)desc.x1 / (float)width;
			float v2 = (float)desc.y1 / (float)height;

			m_textBatchData.insert(m_textBatchData.end(), {x1, y1, 0.0f, u1, v1});
			m_textBatchData.insert(m_textBatchData.end(), {x1, y2, 0.0f, u1, v2});
			m_textBatchData.insert(m_textBatchData.end(), {x2, y2, 0.0f, u2, v2});
			m_textBatchData.insert(m_textBatchData.end(), {x2, y1, 0.0f, u2, v1});

			xOrigin += desc.advance;
		}
	}

} // gui

} // sh