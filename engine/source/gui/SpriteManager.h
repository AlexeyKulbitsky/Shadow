#ifndef SHADOW_SPRITE_MANAGER_INCLUDE
#define SHADOW_SPRITE_MANAGER_INCLUDE

#include "../Globals.h"

namespace sh
{
	
namespace gui
{

	class SpriteManager : public Singleton < SpriteManager >
	{
	public:
		void Load(const pugi::xml_node& node);

		const SpritePtr& GetSprite(const String& name);

	private:
		SpritePtr m_emptySprite;
		Map<String, SpritePtr> m_sprites;
	};

} // gui

} // sh

#endif