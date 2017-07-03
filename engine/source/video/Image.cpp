#include "Image.h"

#include "../Device.h"

#include <stb_image.h>

namespace sh
{

namespace video
{
	Image::Image()
	{

	}

	Image::~Image()
	{

	}

	void Image::Load(const String& name)
	{
		io::File file = io::FileSystem::GetInstance()->LoadFile(name);
		const char* dataPtr = file.GetData().data();
		const u8* uDataPtr = reinterpret_cast<const u8*>(dataPtr);

		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load_from_memory(uDataPtr, file.GetData().size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		SH_ASSERT(pixels, "failed to load texture image!");

		m_width = static_cast<u32>(texWidth);
		m_height = static_cast<u32>(texHeight);
		m_channelsCount = static_cast<u32>(texChannels);

		const u32 size = m_width * m_height * m_channelsCount;
		m_data.resize(size);
		std::copy(pixels, pixels + size, m_data.begin());

		stbi_image_free(pixels);
	}

	void Image::Save(const String& name)
	{

	}

	Color Image::GetPixel(const u32 x, const u32 y) const
	{
		u32 index = m_channelsCount * (y * m_width + x);
		Color color;
		color.red = m_data[index] / 256.0f;
//		color.green = m_data[index + 1] / 256.0f;
//		color.blue = m_data[index + 2] / 256.0f;

		return color;
	}

} // video

} // sh