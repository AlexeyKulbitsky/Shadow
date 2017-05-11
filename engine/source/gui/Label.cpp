#include "Label.h"

namespace sh
{
	
namespace gui
{

	Label::Label()
		: Text()
	{

	}

	Label::Label(const math::Rectu rect)
		: Text(rect)
	{
	}

	Label::Label(const String& text)
		: Text(text)
	{
	}

	Label::Label(const math::Rectu rect, const String& text)
		: Text(rect, text)
	{
	}

	Label::~Label()
	{
	}

} // gui

} // sh