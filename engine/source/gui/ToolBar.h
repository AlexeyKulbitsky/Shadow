#ifndef SHADOW_TOOL_BAR_INCLUDE
#define SHADOW_TOOL_BAR_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class ToolBar : public Widget
	{
	public:
		ToolBar();
		void AddItem(const ButtonPtr& button); 

		virtual void Load(const pugi::xml_node& node) override;
		virtual void GetGeometry(GuiBatchData& data) override;
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

		void OnWindowResized(int width, int height);

	private:
		std::vector<ButtonPtr> m_buttons;
	};

} // gui

} // sh

#endif