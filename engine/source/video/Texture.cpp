#include "Texture.h"
#include "../Device.h"
#include "Driver.h"

namespace sh
{

namespace video
{
	TexturePtr Texture::Create(const TextureDescription& description)
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		return driver->CreateTexture(description);
	}

	Texture::Texture(const TextureDescription& description)
		: m_description(description)
	{

	}
}

} // sh