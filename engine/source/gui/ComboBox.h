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
		ComboBox(const math::Rectu& rect);

		//virtual void GetGeometry(GuiBatchData& data) override;
		//virtual void GetTextGeometry(GuiBatchData& data) override;
		virtual void Render(video::Painter* painter) override;
		virtual void SetPosition(u32 x, u32 y) override;
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

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