#ifndef SHADOW_GUI_STYLE_INCLUDE
#define SHADOW_GUI_STYLE_INCLUDE

#include "../Globals.h"

namespace pugi
{
	class xml_node;
}

namespace sh
{

namespace gui
{

	class SHADOW_API Style
	{
	public:
		void Load(const pugi::xml_node& root);
		void Load(const String& name);

		// Default widgets
		const video::TexturePtr& GetTexure() const { return m_texture; }
		const ButtonPtr& GetButton() const { return m_button; }
		const MenuBarPtr& GetMenuBar() const { return m_menuBar; }
		const ToolBarPtr& GetToolBar() const { return m_toolBar; }
		const WindowPtr& GetWindow() const { return m_window; }
		const LineEditPtr& GetLineEdit() const { return m_lineEdit; }
		const ScrollWidgetPtr& GetScrollWidget() const { return m_scrollWidget; }

		// Custom widgets
		ButtonPtr GetButton(const String& name);
		SpriteWidgetPtr GetSpriteWidget(const String& name);

	private:
		ButtonPtr LoadButton(const pugi::xml_node& node);
		void LoadMenuBar(const pugi::xml_node& node);
		void LoadToolBar(const pugi::xml_node& node);
		void LoadWindow(const pugi::xml_node& node);
		void LoadLineEdit(const pugi::xml_node& node);
		void LoadScrollWidget(const pugi::xml_node& node);
		void LoadCustomButtons(const pugi::xml_node& node);
		void LoadCustomSpriteWidgets(const pugi::xml_node& node);

	private:
		video::TexturePtr m_texture;
		ButtonPtr m_button;
		MenuBarPtr m_menuBar;
		ToolBarPtr m_toolBar;
		WindowPtr m_window;
		LineEditPtr m_lineEdit;
		ScrollWidgetPtr m_scrollWidget;

		// Maps
		Map<String, ButtonPtr> m_buttonsMap;
		Map<String, SpriteWidgetPtr> m_spriteWidgetsMap;
	};

} // gui

} // sh

#endif