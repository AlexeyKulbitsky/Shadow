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
		SHADOW_OBJECT(RenderComponent)
		COMPONENT
	public:
		RenderComponent();
		virtual ~RenderComponent();
		//virtual Type GetType() const override { return Type::Render;}
		virtual void Load(const pugi::xml_node &node) override;
		virtual void Save(pugi::xml_node &parent) override;

		static void RegisterObject();

		void SetModel(scene::ModelPtr model) { m_model = model; }
		const scene::ModelPtr& GetModel() { return m_model; }

		void SetModelProperty(const ResourceRef& value);
		ResourceRef GetModelProperty() const;

	protected:
		scene::ModelPtr m_model;
	};
}

#endif