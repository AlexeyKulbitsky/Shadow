#ifndef SHADOW_SLIDER_WIDGET_INCLUDE
#define SHADOW_SLIDER_WIDGET_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{
	class Button;

	class SliderWidget : public Widget
	{
	public:
		SliderWidget();
		~SliderWidget();

		virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
		virtual void Render(video::Painter* painter) override;
		virtual void UpdateLayout() override;

		float GetValue() const { return m_value; }
		void SetValue(float value);

		Event<void, float> valueChanged;

	private:
		void OnButtonPressed();
		void OnButtonReleased();

	private:
		SPtr<Button> m_button;
		bool m_dragStarted = false;
		sh::math::Vector2i m_startPos;

		math::Vector2f m_valueEdges = math::Vector2f(0.0f, 10.0f);
		float m_value = 0.0f;
	};

} // gui

} // sh

#endif
