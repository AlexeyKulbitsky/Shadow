#include "Label.h"
#include "../serialization/ObjectFactory.h"

namespace sh
{
	
namespace gui
{

	Label::Label()
		: Text()
	{
        m_name = "Label";
	}

	Label::Label(const math::Rect rect)
		: Text(rect)
	{
        m_name = "Label";
	}

	Label::Label(const String& text)
		: Text(text)
	{
        m_name = "Label";
	}

	Label::Label(const math::Rect rect, const String& text)
		: Text(rect, text)
	{
        m_name = "Label";
	}
    
    void Label::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<Label>("UI");
        ObjectFactory::GetInstance()->RegisterParentProperties<Label, Text>();
    }

	Label::~Label()
	{
	}

} // gui

} // sh
