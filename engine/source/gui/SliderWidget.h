#ifndef SHADOW_SLIDER_WIDGET_INCLUDE
#define SHADOW_SLIDER_WIDGET_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{
	class Button;

	class SHADOW_API SliderWidget : public Widget
	{
	public:
		SliderWidget();
		~SliderWidget();

		virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
		virtual void UpdateLayout() override;

		float GetValue() const { return m_value; }
		void SetValue(float value);

		void SetMinValue(float value);
		void SetMaxValue(float value);
		void SetMinMaxValues(float min, float max);
		void SetMinMaxValues(const math::Vector2& values);
		float GetMinValue() const { return m_valueEdges.x; }
		float GetMaxValue() const { return m_valueEdges.y; }
		const math::Vector2&  GetMinMaxValues() const { return m_valueEdges; }

		Event<void, float> valueChanged;

	private:
		SPtr<Button> m_button;
		math::Rect m_sliderRect;
		bool m_dragStarted = false;
		s32 m_dragPointOffset = 0;

		math::Vector2 m_valueEdges = math::Vector2(0.0f, 10.0f);
		float m_value = 0.0f;
	};

} // gui

} // sh

#endif
