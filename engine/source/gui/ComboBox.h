#ifndef SHADOW_COMBOBOX_INCLUDE
#define SHADOW_COMBOBOX_INCLUDE

#include "Widget.h"
#include "Button.h"
#include "VerticalLayout.h"

namespace sh
{

namespace gui
{

	class ComboBox : public Widget
	{
	public:
		ComboBox();
		ComboBox(const math::Recti& rect);

		virtual void Render(video::Painter* painter) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;

		void AddItem(const String& text);
		void RemoveItem(const String& text);
		void RemoveItem(u32 index);

		Event<void, u32> OnItemChanged;

	private:
		void OnButtonToggled(bool toggled);
		void OnItemToggled(bool toggled, const ButtonPtr& sender);
		void UpdateLayout();

	private:
		ButtonPtr m_button;
		ButtonPtr m_currentSelectedItem;
		VerticalLayoutPtr m_layout;

		bool m_showList = false;
	};

} // gui

} // sh

#endif