#ifndef SHADOW_RENDER_COMPONENT_INCLUDE
#define SHADOW_RENDER_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{
	namespace scene
	{
		class Model;
	}

	class SHADOW_API RenderComponent : public Component
	{
		COMPONENT(RenderComponent)
	public:
		RenderComponent();
		virtual ~RenderComponent();

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