#ifndef SHADOW_MENU_BAR_INCLUDE
#define SHADOW_MENU_BAR_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class MenuBar : public Widget
	{
	public:
		MenuBar();
		MenuBar(const SpritePtr& sprite);

		const MenuPtr& AddMenu(const String& menuName, const ButtonPtr& button);

		virtual void GetGeometry(GuiBatchData& data) override;
		virtual void GetTextGeometry(GuiBatchData& data) override;
		virtual void SetPosition(u32 x, u32 y) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;

		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

		void OnWindowResized(int width, int height);

	private:
		void OnMenuReleased();

	private:
		SpritePtr m_sprite;
		std::vector< std::pair<ButtonPtr, MenuPtr> > m_menus;
	};

} // gui

} // 

#endif