#include "FontManager.h"
#include "Font.h"

#include "../Device.h"
#include "../video/Texture.h"
#include "../io/FileSystem.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace sh
{
	FontManager::FontManager()
	{
		FT_Error error = FT_Init_FreeType(&m_fontLibrary);
		SH_ASSERT(!error, "ERROR initializing freetype library!");
	}

	FontPtr FontManager::GenerateFont(const String& filename)
	{
		FT_Face     face;
		FT_Error error;

		const auto file = io::FileSystem::GetInstance()->LoadFile(filename);
		if (file.GetData().size() == 0)
			return FontPtr();
		error = FT_New_Memory_Face(m_fontLibrary, 
								   reinterpret_cast<const u8*>(file.GetData().data()),
								   file.GetData().size(), 0, &face);
		//error = FT_New_Face(m_fontLibrary, fileInefo.absolutePath.c_str(), 0, &face);
		SH_ASSERT(!error, "ERROR loading %s font!", filename.c_str());

		FT_Set_Pixel_Sizes(face, 0, 12);

		FontPtr font(new Font());

		u32 maxDimension = static_cast<u32>((1 + (face->size->metrics.height >> 6)) * ceilf(sqrtf(128)));
		u32 textureWidth = 1;
		while(textureWidth < maxDimension) 
			textureWidth <<= 1; 
		u32 textureHeight = textureWidth; // make square texture with size power of 2

		char* finalBuffer = new char[textureWidth * textureHeight * 4];
		int pen_x = 0, pen_y = 0;

		for (int i = 0; i < 127; ++i)
		{
			FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
			FT_Bitmap* bmp = &face->glyph->bitmap;

			if(pen_x + bmp->width >= textureWidth)
			{
				pen_x = 0;
				pen_y += ((face->size->metrics.height >> 6) + 1);
			}

			for(u32 row = 0; row < bmp->rows; ++row)
			{
				for(u32 col = 0; col < bmp->width; ++col)
				{
					int x = pen_x + col;
					int y = pen_y + row;
					finalBuffer[y * textureWidth * 4 + x * 4] = bmp->buffer[row * bmp->pitch + col];
					finalBuffer[y * textureWidth * 4 + x * 4 + 1] = bmp->buffer[row * bmp->pitch + col];
					finalBuffer[y * textureWidth * 4 + x * 4 + 2] = bmp->buffer[row * bmp->pitch + col];
					finalBuffer[y * textureWidth * 4 + x * 4 + 3] = bmp->buffer[row * bmp->pitch + col];
				}
			}

			// this is stuff you'd need when rendering individual glyphs out of the atlas

			font->m_descriptions[i].x0 = pen_x;
			font->m_descriptions[i].y0 = pen_y;
			font->m_descriptions[i].x1 = pen_x + bmp->width;
			font->m_descriptions[i].y1 = pen_y + bmp->rows;

			font->m_descriptions[i].x_off   = face->glyph->bitmap_left;
			font->m_descriptions[i].y_off   = face->glyph->bitmap_top;
			font->m_descriptions[i].width   = bmp->width;
			font->m_descriptions[i].height  = bmp->rows;
			font->m_descriptions[i].advance = face->glyph->advance.x >> 6;

			pen_x += bmp->width + 1;
		}

		video::TextureDescription textureDesc;
		textureDesc.type = TEX_TYPE_TEXTURE_2D;
		textureDesc.width = textureWidth;
		textureDesc.height = textureHeight;
		textureDesc.format = TextureFormat::RGBA;// TextureFormat::ALPHA;

		m_texture = video::Texture::Create(textureDesc);
		m_texture->SetData(0U, finalBuffer);

		font->m_textureatlas = m_texture;

#if 0
		// write png
		char* png_data = new char[textureWidth * textureHeight * 4];
		for(int i = 0; i < (textureWidth * textureHeight); ++i)
		{
			png_data[i * 4 + 0] |= finalBuffer[i];
			png_data[i * 4 + 1] |= finalBuffer[i];
			png_data[i * 4 + 2] |= finalBuffer[i];
			png_data[i * 4 + 3] = 0xff;
		}

		stbi_write_png("font_output.png", textureWidth, textureHeight, 4, png_data, textureWidth * 4);

		delete[] png_data;
#endif

		delete[] finalBuffer;

		return font;
	}
}