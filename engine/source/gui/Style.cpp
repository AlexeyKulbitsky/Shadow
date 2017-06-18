#include "Style.h"

#include "Sprite.h"
#include "Button.h"

#include "../Device.h"
#include "../resources/ResourceManager.h"

namespace sh
{
	 
namespace gui
{

	void Style::Load(const pugi::xml_node& root)
	{
		pugi::xml_node textureAtlasNode = root.child("texture");
		SH_ASSERT(!textureAtlasNode.empty(), "Empty texture atlas!");
		const String textureName = textureAtlasNode.attribute("val").as_string();

		auto resManager = Device::GetInstance()->GetResourceManager();
		SH_ASSERT(resManager, "No resourece manager instance!");

		m_texture = resManager->GetTexture(textureName);
		SH_ASSERT(m_texture, "No atlas texture for GUI style!");

		pugi::xml_node buttonNode = root.child("button");
		if (buttonNode)
			LoadButtonStyle(buttonNode);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void Style::LoadButtonStyle(const pugi::xml_node& node)
	{
		const u32 statesCount = 3U;
		std::array<String, statesCount> states = { "default", "pressed", "hovered" };
		std::array<SpritePtr, statesCount> sprites;

		for (u32 i = 0U; i < statesCount; ++i)
		{
			pugi::xml_node stateNode = node.child(states[i].c_str());
			pugi::xml_node rectNode = stateNode.child("rect");
			u32 x1 = rectNode.attribute("x1").as_uint();
			u32 y1 = rectNode.attribute("y1").as_uint();
			u32 x2 = rectNode.attribute("x2").as_uint();
			u32 y2 = rectNode.attribute("y2").as_uint();
			math::Rectu rect(x1, y1, x2, y2);
			sprites[i].reset(new Sprite(m_texture, rect));
		}
		
		m_button.reset(new Button(sprites[0], sprites[1], sprites[2]));
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh