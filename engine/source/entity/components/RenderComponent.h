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

		void SetModel(scene::ModelPtr model);
		const scene::ModelPtr& GetModel();

		void SetModelProperty(const ResourceRef& value);
		ResourceRef GetModelProperty() const;

		void SetMaterials(const NamedResourceRefList& value);
		const NamedResourceRefList& GetMaterials() const;

	private:
		void UpdateMaterials();

	protected:
		scene::ModelPtr m_model;

		NamedResourceRefList m_namedResourceRefList;
	};
}

#endif