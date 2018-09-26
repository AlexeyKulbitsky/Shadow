#ifndef SHADOW_MENU_BAR_INCLUDE
#define SHADOW_MENU_BAR_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class SHADOW_API MenuBar : public Widget
	{
        SHADOW_OBJECT(MenuBar)
	public:
		MenuBar();
		MenuBar(const SpritePtr& sprite);
        
        static void RegisterObject();

		void SetToggled(bool toggled);
		bool IsToggled() const { return m_toggled; }

		const MenuPtr& AddMenu(const String& menuName, const ButtonPtr& button);
		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;
		virtual void AdjustSize() override;

	private:
		class MenuBarItem : public Widget
		{
		public:
			MenuBarItem(const String& menuName, const ButtonPtr& button);
			virtual void UpdateLayout() override;
			virtual void Render(video::Painter* painter) override;
			virtual void RenderBackground(video::Painter* painter) override;
			virtual void RenderText(video::Painter* painter) override;
			virtual bool ProcessEvent(GUIEvent& ev) override;
			void OnButtonToggled(bool toggled, const ButtonPtr& sender);
			void OnButtonHovered(const ButtonPtr& sender);
			void OnMenuItemSelected(const String& itemName);

			ButtonPtr button;
			MenuPtr menu;
			MenuBar* parent = nullptr;
		};
		DEFINE_CLASS_PTR(MenuBarItem)

		SpritePtr m_sprite;
		std::vector< std::pair<ButtonPtr, MenuPtr> > m_menus;

		std::vector<MenuBarItemPtr> m_menuItems;
		MenuBarItem* m_activeItem = nullptr;
		bool m_toggled = false;
	};

} // gui

} // 

#endif
