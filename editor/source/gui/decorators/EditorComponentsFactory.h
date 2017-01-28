#ifndef SHADOW_EDITOR_COMPONENTS_FACTORY_INCLUDE
#define SHADOW_EDITOR_COMPONENTS_FACTORY_INCLUDE

#include <Shadow.h>

class EditorComponentsFactory : public sh::ComponentsFactory
{
public:
	virtual ~EditorComponentsFactory();
	virtual sh::Component* CreateComponent(sh::Component::Type type) override;	
};

#endif