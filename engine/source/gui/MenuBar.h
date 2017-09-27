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
		virtual void Render(video::Painter* painter) override;

	private:
		class MenuBarItem : public Widget
		{
		public:
			MenuBarItem(const String& menuName, const ButtonPtr& button);
			virtual void UpdateLayout() override;
			virtual void Render(video::Painter* painter) override;
			virtual bool ProcessEvent(GUIEvent& ev) override;
			void OnButtonToggled(bool toggled, const ButtonPtr& sender);

			ButtonPtr button;
			MenuPtr menu;
		};
		DEFINE_CLASS_PTR(MenuBarItem)

		SpritePtr m_sprite;
		std::vector< std::pair<ButtonPtr, MenuPtr> > m_menus;

		std::vector<MenuBarItemPtr> m_menuItems;
	};

} // gui

} // 

#endif