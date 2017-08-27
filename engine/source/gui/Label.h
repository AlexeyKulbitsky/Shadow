#ifndef SHADOW_LABEL_INCLUDE
#define SHADOW_LABEL_INCLUDE

#include "Text.h"

namespace sh
{

namespace gui
{

	class Label : public Text
	{
	public:
		Label(); 
		Label(const math::Recti rect);
		Label(const String& text);
		Label(const math::Recti rect, const String& text);

		virtual ~Label();
	};

} // gui

} // sh

#endif