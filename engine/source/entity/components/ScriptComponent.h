#ifndef SHADOW_SCRIPT_COMPONENT_INCLUDE
#define SHADOW_SCRIPT_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{
	class Script;

	class SHADOW_API ScriptComponent : public Component
	{
		COMPONENT(ScriptComponent)
	public:

		ScriptComponent();
		virtual ~ScriptComponent();

		static void RegisterObject();

		virtual Component* Clone() override;

		void SetScript(Script* script);
		Script* GetScript();

		void SetScriptRef(const ScriptRef& ref);
		const ScriptRef GetScriptRef() const;

		void Update(f32 deltaTime);

	private:
		Script* m_script = nullptr;

	};

} // 

#endif
