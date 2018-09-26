#ifndef SHADOW_FLOAT_LINE_EDIT_INCLUDE
#define SHADOW_FLOAT_LINE_EDIT_INCLUDE

#include "LineEdit.h"

namespace sh
{

namespace gui
{

	class SHADOW_API FloatLineEdit : public LineEdit
	{
        SHADOW_OBJECT(FloatLineEdit)
	public:
		FloatLineEdit();
		FloatLineEdit(const math::Rect& rect, 
					const SpritePtr& defaultSprite, 
					const SpritePtr& editSprite);
        
        static void RegisterObject();

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
