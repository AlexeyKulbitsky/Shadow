#ifndef SHADOW_GUI_ELEMENT_INCLUDE
#define SHADOW_GUI_ELEMENT_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{
	
	class GuiElement
	{
	public:
		void SetPosition(u32 x, u32 y);
		void SetWidth(u32 width);
		void SetHeight(u32 height);
	};

} // gui

} // sh

#endif