#ifndef SHADOW_INT_LINE_EDIT_INCLUDE
#define SHADOW_INT_LINE_EDIT_INCLUDE

#include "LineEdit.h"

namespace sh
{

	namespace gui
	{

		class IntLineEdit : public LineEdit
		{
		public:
			IntLineEdit();
			IntLineEdit(const math::Recti& rect,
				const SpritePtr& defaultSprite,
				const SpritePtr& editSprite);

			virtual bool ProcessEvent(GUIEvent& ev) override;

			int GetValue() const { return m_value; }
			void SetValue(int value);

			Event<void, int> OnValueChanged;

		protected:
			bool CheckIntFilter(const String& string);
			virtual void UpdateIfDirty() override;

		private:
			int m_value = 0;
		};

	} // gui

} // sh

#endif