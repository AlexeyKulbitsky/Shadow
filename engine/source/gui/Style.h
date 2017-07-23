#ifndef SHADOW_GUI_STYLE_INCLUDE
#define SHADOW_GUI_STYLE_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{

	class Style
	{
	public:
		void Load(const pugi::xml_node& root);

		const video::TexturePtr& GetTexure() const { return m_texture; }
		const ButtonPtr& GetButton() const { return m_button; }
		const MenuBarPtr& GetMenuBar() const { return m_menuBar; }
		const WindowPtr& GetWindow() const { return m_window; }
		const LineEditPtr& GetLineEdit() const { return m_lineEdit; }

	private:
		void LoadButtonStyle(const pugi::xml_node& node);
		void LoadMenuBar(const pugi::xml_node& node);
		void LoadWindow(const pugi::xml_node& node);
		void LoadLineEdit(const pugi::xml_node& node);

	private:
		video::TexturePtr m_texture;
		ButtonPtr m_button;
		MenuBarPtr m_menuBar;
		WindowPtr m_window;
		LineEditPtr m_lineEdit;
	};

} // gui

} // sh

#endif