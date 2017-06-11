#include "Layout.h"

namespace sh
{

namespace gui
{

	void Layout::AddElement(const GuiElementPtr& element)
	{
		m_children.push_back(element);
	}

} // gui

} // sh