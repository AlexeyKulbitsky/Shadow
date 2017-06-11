#ifndef SHADOW_VECTOR3_LINE_EDIT_INCLUDE
#define SHADOW_VECTOR3_LINE_EDIT_INCLUDE

#include <gui/FloatLineEdit.h>

class Vector3LineEdit : public sh::gui::GuiElement
{
public:
	Vector3LineEdit();

private:
	std::array<sh::gui::FloatLineEditPtr, 3U> m_lineEdits;
};

#endif