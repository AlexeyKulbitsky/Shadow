#include "Style.h"

#include "Sprite.h"
#include "Button.h"
#include "MenuBar.h"
#include "Window.h"
#include "LineEdit.h"

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

		pugi::xml_node menuBarNode = root.child("menubar");
		if (menuBarNode)
			LoadMenuBar(menuBarNode);

		pugi::xml_node windowNode = root.child("window");
		if (windowNode)
			LoadWindow(windowNode);

		pugi::xml_node lineEditNode = root.child("lineedit");
		if (lineEditNode)
			LoadLineEdit(lineEditNode);
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

	void Style::LoadMenuBar(const pugi::xml_node& node)
	{
		pugi::xml_node rectNode = node.child("rect");
		u32 x1 = rectNode.attribute("x1").as_uint();
		u32 y1 = rectNode.attribute("y1").as_uint();
		u32 x2 = rectNode.attribute("x2").as_uint();
		u32 y2 = rectNode.attribute("y2").as_uint();
		math::Rectu rect(x1, y1, x2, y2);
		SpritePtr sprite(new Sprite(m_texture, rect));

		m_menuBar.reset(new MenuBar(sprite));
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void Style::LoadWindow(const pugi::xml_node& node)
	{
		const u32 rectsCount = 3U;
		std::array<String, rectsCount> rects = { "outrect", "inrect", "barrect" };
		std::array<SpritePtr, rectsCount> sprites;

		for (u32 i = 0U; i < rectsCount; ++i)
		{
			pugi::xml_node rectNode = node.child(rects[i].c_str());
			u32 x1 = rectNode.attribute("x1").as_uint();
			u32 y1 = rectNode.attribute("y1").as_uint();
			u32 x2 = rectNode.attribute("x2").as_uint();
			u32 y2 = rectNode.attribute("y2").as_uint();
			math::Rectu rect(x1, y1, x2, y2);
			sprites[i].reset(new Sprite(m_texture, rect));
		}

		m_window.reset(new Window(sprites[0], sprites[1], sprites[2]));
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void Style::LoadLineEdit(const pugi::xml_node& node)
	{
		const u32 rectsCount = 2U;
		std::array<String, rectsCount> rects = { "default", "edit" };
		std::array<SpritePtr, rectsCount> sprites;

		for (u32 i = 0U; i < rectsCount; ++i)
		{
			pugi::xml_node stateNode = node.child(rects[i].c_str());
			pugi::xml_node rectNode = stateNode.child("rect");
			u32 x1 = rectNode.attribute("x1").as_uint();
			u32 y1 = rectNode.attribute("y1").as_uint();
			u32 x2 = rectNode.attribute("x2").as_uint();
			u32 y2 = rectNode.attribute("y2").as_uint();
			math::Rectu rect(x1, y1, x2, y2);
			sprites[i].reset(new Sprite(m_texture, rect));
		}

		m_lineEdit.reset(new LineEdit(sprites[0], sprites[1]));
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh