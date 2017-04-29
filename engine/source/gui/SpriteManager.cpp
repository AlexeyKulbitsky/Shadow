#include "SpriteManager.h"

#include "Sprite.h"

#include "../Device.h"

namespace sh
{

namespace gui
{

	void SpriteManager::Load(const pugi::xml_node& node)
	{
		pugi::xml_node child = node.child("sprite");

		while (child)
		{
			String name = child.attribute("name").as_string();

			pugi::xml_node rect = child.child("rect");
			SH_ASSERT(rect, "Can not find Rect node for sprite!");

			u32 x1 = rect.attribute("x1").as_uint();
			u32 y1 = rect.attribute("y1").as_uint();
			u32 x2 = rect.attribute("x2").as_uint();
			u32 y2 = rect.attribute("y2").as_uint();
			math::Rectu r(x1, y1, x2, y2);

			pugi::xml_node tex = child.child("texture");
			SH_ASSERT(rect, "Can not find Rect node for sprite!");

			sh::String texFilename = tex.attribute("val").as_string();
			sh::video::TexturePtr texture = sh::Device::GetInstance()->GetResourceManager()->GetTexture(texFilename);

			SpritePtr sprite(new Sprite(texture, r));
			sprite->SetName(name);

			m_sprites[name] = sprite;
			child = child.next_sibling();
		}
	}

	const SpritePtr& SpriteManager::GetSprite(const String& name)
	{
		if (m_sprites.find(name) != m_sprites.end())
			return m_sprites.at(name);

		return m_emptySprite;
	}

} // gui

} // sh