#ifndef SHADOW_LABEL_INCLUDE
#define SHADOW_LABEL_INCLUDE

#include "Text.h"

namespace sh
{

namespace gui
{

	class SHADOW_API Label : public Text
	{
        SHADOW_OBJECT(Label)
	public:
		Label(); 
		Label(const math::Rect rect);
		Label(const String& text);
		Label(const math::Rect rect, const String& text);
        
        static void RegisterObject();

		virtual ~Label();
	};

} // gui

} // sh

#endif
