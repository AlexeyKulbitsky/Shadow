#ifndef SCRIPT_A_INCLUDE
#define SCRIPT_A_INCLUDE

#include <Shadow.h>

class ScriptA : public sh::Script
{
	SHADOW_OBJECT(ScriptA)
public:
	static void RegisterObject();
	virtual void Update(float dt) override;
};

#endif
