#ifndef SHADOW_FLOAT_LINE_EDIT_INCLUDE
#define SHADOW_FLOAT_LINE_EDIT_INCLUDE

#include "LineEdit.h"

namespace sh
{

namespace gui
{

	class FloatLineEdit : public LineEdit
	{
	public:
		FloatLineEdit();
		FloatLineEdit(const math::Recti& rect, 
					const SpritePtr& defaultSprite, 
					const SpritePtr& editSprite);

		virtual bool ProcessEvent(GUIEvent& ev) override;
	
		float GetValue() const { return m_value; }
		void SetValue(float value);

		Event<void, float> OnValueChanged;

	protected:
		bool CheckFloatFilter(const String& string);
		virtual void UpdateIfDirty() override;

	private:
		float m_value = 0.0f;
	};

} // gui

} // sh

#endif