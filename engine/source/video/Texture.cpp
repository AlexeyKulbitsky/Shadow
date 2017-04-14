#include "Texture.h"
#include "Managers/TextureManager.h"

namespace sh
{

namespace video
{
	TexturePtr Texture::Create(const TextureDescription& description)
	{
		return TextureManager::GetInstance()->CreateTexture(description);
	}

	Texture::Texture(const TextureDescription& description)
		: m_description(description)
	{

	}
}

} // sh