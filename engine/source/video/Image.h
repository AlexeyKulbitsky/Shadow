#ifndef SHADOW_IMAGE_INCLUDE
#define SHADOW_IMAGE_INCLUDE

#include "../Globals.h"
#include "Color.h"

namespace sh
{

namespace video
{

	class SHADOW_API Image
	{
	public:
		Image();
		~Image();

		void Load(const String& name);
		void Save(const String& name);

		u32 GetWidth() const { return m_width; }
		u32 GetHeight() const { return m_height; }
		u32 GetChannelsCount() const { return m_channelsCount; }
		Color GetPixel(const u32 x, const u32 y) const;
		const std::vector<u8>& GetData() const { return m_data; }

	private:
		u32 m_width = 0U;
		u32 m_height = 0U;
		u32 m_channelsCount = 0U;
		std::vector<u8> m_data;
	};

} // video

} // sh

#endif