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
		virtual void Save(pugi::xml_node &parent) override;

		void SetModel(scene::ModelPtr model) { m_model = model; }
		const scene::ModelPtr& GetModel() { return m_model; }

	protected:
		scene::ModelPtr m_model;
	};
}

#endif