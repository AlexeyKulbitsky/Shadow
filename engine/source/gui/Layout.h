#ifndef SHADOW_LAYOUT_INCLUDE
#define SHADOW_LAYOUT_INCLUDE

#include "GuiElement.h"

namespace sh
{

namespace gui
{

	class Layout : public GuiElement
	{
	public:
		void AddElement(const GuiElementPtr& element);

	protected:
		std::vector<GuiElementPtr> m_children;
	};

} // gui

} // sh

#endif