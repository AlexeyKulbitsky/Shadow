#include "Style.h"

#include "Sprite.h"
#include "Button.h"
#include "MenuBar.h"
#include "ToolBar.h"
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
			m_button = LoadButton(buttonNode);

		pugi::xml_node menuBarNode = root.child("menubar");
		if (menuBarNode)
			LoadMenuBar(menuBarNode);

		pugi::xml_node toolBarNode = root.child("toolbar");
		if (toolBarNode)
			LoadToolBar(toolBarNode);

		pugi::xml_node windowNode = root.child("window");
		if (windowNode)
			LoadWindow(windowNode);

		pugi::xml_node lineEditNode = root.child("lineedit");
		if (lineEditNode)
			LoadLineEdit(lineEditNode);

		pugi::xml_node customButtonsNode = root.child("buttons");
		if (customButtonsNode)
			LoadCustomButtons(customButtonsNode);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	ButtonPtr Style::GetButton(const String& name)
	{
		auto it = m_buttonsMap.find(name);
		if (it == m_buttonsMap.end())
			return ButtonPtr();

		return m_buttonsMap.at(name);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	ButtonPtr Style::LoadButton(const pugi::xml_node& node)
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

			math::Vector4f color(1.0f);
			pugi::xml_node colorNode = stateNode.child("color");
			if (colorNode)
			{
				color.x = colorNode.attribute("r").as_float();
				color.y = colorNode.attribute("g").as_float();
				color.z = colorNode.attribute("b").as_float();
				color.w = colorNode.attribute("a").as_float();
			}
			sprites[i].reset(new Sprite(m_texture, rect, color));
		}
		
		ButtonPtr result(new Button(sprites[0], sprites[1], sprites[2]));
		return result;
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

	void Style::LoadToolBar(const pugi::xml_node& node)
	{
		pugi::xml_node rectNode = node.child("rect");
		u32 x1 = rectNode.attribute("x1").as_uint();
		u32 y1 = rectNode.attribute("y1").as_uint();
		u32 x2 = rectNode.attribute("x2").as_uint();
		u32 y2 = rectNode.attribute("y2").as_uint();
		math::Rectu rect(x1, y1, x2, y2);
		SpritePtr sprite(new Sprite(m_texture, rect));

		m_toolBar.reset(new ToolBar(sprite));
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

	void Style::LoadCustomButtons(const pugi::xml_node& node)
	{
		pugi::xml_node child = node.child("button");
		while (child)
		{
			String name = child.attribute("name").as_string();
			ButtonPtr button = LoadButton(child);

			m_buttonsMap[name] = button;

			child = child.next_sibling();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh