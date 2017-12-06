#include "ScriptA.h"

#include <iostream>

void ScriptA::RegisterObject()
{
	sh::ObjectFactory::GetInstance()->RegisterFactory<ScriptA>("Scripts");
}

void ScriptA::Update(float dt)
{
	std::cout << "Updating SCRIPT A : " << dt << std::endl;
}