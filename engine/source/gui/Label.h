#ifndef SHADOW_LABEL_INCLUDE
#define SHADOW_LABEL_INCLUDE

#include "Text.h"

namespace sh
{

namespace gui
{

	class SHADOW_API Label : public Text
	{
	public:
		Label(); 
		Label(const math::Rect rect);
		Label(const String& text);
		Label(const math::Rect rect, const String& text);

		virtual ~Label();
	};

} // gui

} // sh

#endif