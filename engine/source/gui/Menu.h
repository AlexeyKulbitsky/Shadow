#ifndef SHADOW_MENU_INCLUDE
#define SHADOW_MENU_INCLUDE

#include "GuiElement.h"

namespace sh
{

namespace gui
{

	class Menu : public GuiElement
	{
	public:
		Menu();
		Menu(const String& title);

		void Show();
		void Hide();

		void SetTitle(const String& title) { m_title = title; }
		const String& GetTitle() const { return m_title; }

		void AddItem(const ButtonPtr& button);// { m_buttons.push_back(button); }

		virtual void GetGeometry(GuiBatchData& data) override;
		virtual void GetTextGeometry(GuiBatchData& data) override;
		virtual void SetPosition(u32 x, u32 y) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;

		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

	private:
		String m_title;
		std::vector<ButtonPtr> m_buttons;
	};

} // gui

} // sh

#endif