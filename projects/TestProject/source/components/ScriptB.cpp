#include "ScriptB.h"

#include <iostream>

void ScriptB::RegisterObject()
{
	sh::ObjectFactory::GetInstance()->RegisterFactory<ScriptB>("Scripts");
}

void ScriptB::Update(float dt)
{
	std::cout << "Updating SCRIPT B : " << dt << std::endl;
}

sh::Script* ScriptB::Clone()
{
	return new ScriptB();
}
