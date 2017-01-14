#ifndef SHADOW_RENDER_COMPONENT_INCLUDE
#define SHADOW_RENDER_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{
	namespace scene
	{
		class Model;
	}

	class RenderComponent : public Component
	{
	public:
		RenderComponent();
		virtual ~RenderComponent();
		virtual Type GetType() const override { return Type::RENDER;}
		virtual void Load(const pugi::xml_node &node) override;

		void SetModel(scene::Model* model) { m_model = model; }
		scene::Model* GetModel() { return m_model; }

	protected:
		scene::Model* m_model;
	};
}

#endif