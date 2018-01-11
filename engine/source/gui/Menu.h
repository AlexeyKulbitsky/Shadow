#ifndef SHADOW_MENU_INCLUDE
#define SHADOW_MENU_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class SHADOW_API Menu : public Widget
	{
	public:
		Menu();
		Menu(const String& title);
		virtual ~Menu();

		void Show();
		void Hide();

		void SetTitle(const String& title) { m_title = title; }
		const String& GetTitle() const { return m_title; }

		void AddItem(const ButtonPtr& button);
		void AddItem(const String& name);
		void AddSubmenu(const String& itemName, const MenuPtr& subMenu);
		void AddSubmenu(size_t itemIndex, const MenuPtr& subMenu);

		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;

		virtual bool ProcessEvent(GUIEvent& ev) override;
		virtual void UpdateLayout() override;
		virtual void AdjustSize() override;

		Event<void, const String&> itemSelected;
		Event<void, size_t> itemIndexSelected;

	private:
		void OnButtonReleased(const ButtonPtr& sender);
		void OnButtonHovered(const ButtonPtr& sender);
		void OnSubmenuItemSelected(const String& itemName);

	private:
		String m_title;
		std::vector<ButtonPtr> m_buttons;
		struct SubmenuItem
		{
			Button* button = nullptr;
			MenuPtr menu;
		};

		std::vector<SubmenuItem> m_submenus;
	};

} // gui

} // sh

#endif