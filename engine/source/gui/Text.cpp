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

	Text::Text(const math::Recti rect)
	{
		m_rect = rect;
	}

	Text::Text(const String& text)
	{
		SetText(text);
	}

	Text::Text(const math::Recti rect, const String& text)
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
	}

	void Text::Render(video::Painter* painter)
	{
		if (!m_visible)
			return;

		if (m_glyphOffsets.size() != m_text.size())
			m_glyphOffsets.resize(m_text.size());

		painter->SetMaterial(GuiManager::GetInstance()->GetTextMaterial());
		//const auto cachedClipRect = painter->GetClipRect();

		//painter->SetClipRect(math::Rectu(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y,
		//	m_rect.lowerRightCorner.x, m_rect.lowerRightCorner.y));

		const auto& viewPort = sh::Device::GetInstance()->GetDriver()->GetViewPort();

		const auto& font = GuiManager::GetInstance()->GetFont();
		s32 width = font->GetTextureAtlas()->GetDescription().width; 
		s32 height = font->GetTextureAtlas()->GetDescription().height;

		s32 xOrigin = m_rect.upperLeftCorner.x + 5;
		s32 yOrigin = m_rect.lowerRightCorner.y - m_rect.GetHeight() / 4;
		size_t counter = 0U;
		for (const auto c : m_text)
		{
			m_glyphOffsets[counter++] = xOrigin;
			const auto& desc = font->GetGlyphDescription(static_cast<u32>(c));
			
			float x1 = static_cast<float>(xOrigin + desc.x_off);
			float y1 = static_cast<float>(yOrigin - desc.y_off);
			float x2 = static_cast<float>(xOrigin + desc.x_off + desc.width);
			float y2 = static_cast<float>(yOrigin - desc.y_off + desc.height);

			float u1 = (float)desc.x0 / (float)width;
			float v1 = (float)desc.y0 / (float)height;
			float u2 = (float)desc.x1 / (float)width;
			float v2 = (float)desc.y1 / (float)height;

			video::Painter::Vertex upperLeft(math::Vector3f(x1, y1, 0.0f), 
											math::Vector2f(u1, v1), 
											math::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
			video::Painter::Vertex downRight(math::Vector3f(x2, y2, 0.0f),
											math::Vector2f(u2, v2),
											math::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
			painter->DrawRect(upperLeft, downRight);

			xOrigin += desc.advance;
		}
		//painter->SetClipRect(cachedClipRect);
	}

	void Text::SetPosition( s32 x, s32 y )
	{
		auto size = m_rect.GetSize();
		m_rect.Set(x, y, x + size.x, y + size.y);
	}

	void Text::SetSize( const math::Vector2i& size )
	{
		Widget::SetSize(size);
	}

	void Text::SetWidth( s32 width )
	{
		Widget::SetWidth(width);
	}

	void Text::SetHeight( s32 height )
	{
		Widget::SetHeight(height);
	}

} // gui

} // sh