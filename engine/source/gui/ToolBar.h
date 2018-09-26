#ifndef SHADOW_TOOL_BAR_INCLUDE
#define SHADOW_TOOL_BAR_INCLUDE

#include "Widget.h"

namespace pugi
{
	class xml_node;
}

namespace sh
{

namespace gui
{

	class SHADOW_API ToolBar : public Widget
	{
        SHADOW_OBJECT(ToolBar)
	public:
		ToolBar();
		ToolBar(const SpritePtr& sprite);
        
        static void RegisterObject();
		void AddItem(const ButtonPtr& button); 

		virtual void Load(const pugi::xml_node& node) override;
		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;
		virtual void AdjustSize() override;

	private:
		SpritePtr m_sprite;
	};

} // gui

} // sh

#endif
