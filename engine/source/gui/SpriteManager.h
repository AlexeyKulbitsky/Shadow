#ifndef SHADOW_SPRITE_MANAGER_INCLUDE
#define SHADOW_SPRITE_MANAGER_INCLUDE

#include "../Globals.h"

namespace pugi
{
	class xml_node;
}

namespace sh
{
	
namespace gui
{

	class SHADOW_API SpriteManager : public Singleton < SpriteManager >
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