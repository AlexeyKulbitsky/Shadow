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

	private:
		void LoadButtonStyle(const pugi::xml_node& node);

	private:
		video::TexturePtr m_texture;
		ButtonPtr m_button;
	};

} // gui

} // sh

#endif