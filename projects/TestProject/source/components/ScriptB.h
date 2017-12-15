#ifndef SCRIPT_B_INCLUDE
#define SCRIPT_B_INCLUDE

#include <Shadow.h>

class ScriptB : public sh::Script
{
	SHADOW_OBJECT(ScriptB)
public:
	static void RegisterObject();
	virtual void Update(float dt) override;

	virtual sh::Script* Clone() override;
};

#endif
