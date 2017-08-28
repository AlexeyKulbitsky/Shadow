#ifndef SHADOW_MENU_INCLUDE
#define SHADOW_MENU_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class Menu : public Widget
	{
	public:
		Menu();
		Menu(const String& title);

		void Show();
		void Hide();

		void SetTitle(const String& title) { m_title = title; }
		const String& GetTitle() const { return m_title; }

		void AddItem(const ButtonPtr& button);

		virtual void Render(video::Painter* painter) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;

		virtual bool ProcessEvent(GUIEvent& ev) override;

	private:
		String m_title;
		std::vector<ButtonPtr> m_buttons;
	};

} // gui

} // sh

#endif