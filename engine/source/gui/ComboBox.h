#ifndef SHADOW_COMBOBOX_INCLUDE
#define SHADOW_COMBOBOX_INCLUDE

#include "Widget.h"
#include "Button.h"
#include "VerticalLayout.h"
#include "ScrollWidget.h"

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
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;

		void AddItem(const String& text);
		void RemoveItem(const String& text);
		void RemoveItem(u32 index);
		void SetSelectedItem(const String& item);
		void SetSelectedItem(const u32 index);

		Event<void, u32> OnItemChanged;

	private:
		void OnButtonToggled(bool toggled);
		void OnItemToggled(bool toggled, const ButtonPtr& sender);
		void UpdateLayout();

	private:
		ButtonPtr m_button;
		ButtonPtr m_currentSelectedItem;
		VerticalLayoutPtr m_layout;
		ScrollWidgetPtr m_scrollWidget;

		bool m_showList = false;
	};

} // gui

} // sh

#endif