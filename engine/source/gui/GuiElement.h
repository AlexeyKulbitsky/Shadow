#ifndef SHADOW_GUI_ELEMENT_INCLUDE
#define SHADOW_GUI_ELEMENT_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{
	struct GeometryData
	{
		u32 offset = 0U;
	};

	class GuiElement
	{
	public:
		virtual void SetPosition(u32 x, u32 y) = 0;
		virtual void SetWidth(u32 width) = 0;
		virtual void SetHeight(u32 height) = 0;

		virtual bool ProcessInput(u32 x, u32 y) = 0;
	};

} // gui

} // sh

#endif