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
		UpdateColor(math::Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
	}

	Text::Text(const math::Rectu rect)
	{
		m_rect = rect;
		UpdateColor(math::Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
	}

	Text::Text(const String& text)
	{
		SetText(text);
		UpdateColor(math::Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
	}

	Text::Text(const math::Rectu rect, const String& text)
	{
		m_rect = rect;
		SetText(text);
		UpdateColor(math::Vector4f(1.0f, 1.0f, 1.0f, 0.0f));
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
		const auto& viewPort = sh::Device::GetInstance()->GetDriver()->GetViewPort();

		const auto& font = GuiManager::GetInstance()->GetFont();
		s32 width = font->GetTextureAtlas()->GetDescription().width; 
		s32 height = font->GetTextureAtlas()->GetDescription().height;

		s32 xOrigin = m_rect.upperLeftCorner.x + 5;
		s32 yOrigin = m_rect.lowerRightCorner.y - m_rect.GetHeight() / 4;

		for (const auto c : m_text)
		{
			const auto& desc = font->GetGlyphDescription(static_cast<u32>(c));
			
			float x1 = static_cast<float>(xOrigin + desc.x_off);
			float y1 = static_cast<float>(yOrigin - desc.y_off);
			float x2 = static_cast<float>(xOrigin + desc.x_off + desc.width);
			float y2 = static_cast<float>(yOrigin - desc.y_off + desc.height);

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

	void Text::SetPosition( u32 x, u32 y )
	{
		auto size = m_rect.GetSize();
		m_rect.Set(x, y, x + size.x, y + size.y);
		UpdateTextGeometry();
	}

	void Text::SetSize( const math::Vector2u& size )
	{
		Widget::SetSize(size);
		UpdateTextGeometry();
	}

	void Text::SetWidth( u32 width )
	{
		Widget::SetWidth(width);
		UpdateTextGeometry();
	}

	void Text::SetHeight( u32 height )
	{
		Widget::SetHeight(height);
		UpdateTextGeometry();
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
			
			float x1 = static_cast<float>(xOrigin + desc.x_off);
			float y1 = static_cast<float>(yOrigin - desc.y_off);
			float x2 = static_cast<float>(xOrigin + desc.x_off + desc.width);
			float y2 = static_cast<float>(yOrigin - desc.y_off + desc.height);

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