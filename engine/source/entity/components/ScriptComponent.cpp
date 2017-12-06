#include "ScriptComponent.h"

#include "../../scripting/Script.h"
#include "../../serialization/ObjectFactory.h"

namespace sh
{

	ScriptComponent::ScriptComponent()
	{
	}

	ScriptComponent::~ScriptComponent()
	{
	}

	void ScriptComponent::RegisterObject()
	{
		ObjectFactory::GetInstance()->RegisterFactory<ScriptComponent>("Components");
		S_ACCESSOR_PROPERTY("Script", GetScriptRef, SetScriptRef);
	}

	void ScriptComponent::SetScript(Script* script)
	{
		m_script = script;
	}

	Script* ScriptComponent::GetScript()
	{
		return m_script;
	}

	void ScriptComponent::SetScriptRef(const ScriptRef& ref)
	{
		auto object = ObjectFactory::GetInstance()->CreateObject(ref.type);
		if (!object)
			return;

		m_script = static_cast<Script*>(object);
	}

	const ScriptRef ScriptComponent::GetScriptRef() const
	{
		ScriptRef ref;
		if (!m_script)
			return ref;

		ref.type = m_script->GetTypeName();
		return ref;
	}

	void ScriptComponent::Update(f32 deltaTime)
	{
		if (m_script)
			m_script->Update(deltaTime);
	}

} // sh