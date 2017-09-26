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
		ToolBar(const SpritePtr& sprite);
		void AddItem(const ButtonPtr& button); 

		virtual void Load(const pugi::xml_node& node) override;
		virtual void Render(video::Painter* painter) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;

	private:
		std::vector<ButtonPtr> m_buttons;
		SpritePtr m_sprite;
	};

} // gui

} // sh

#endif