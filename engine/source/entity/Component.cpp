#include "Component.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/LightComponent.h"
#include "components/TerrainComponent.h"

namespace sh
{
	size_t IdCounter::counter = 0;

	Component* Component::Create(Component::Type type)
	{
		switch (type)
		{
			case Type::Transform:
				return new TransformComponent();
			case Type::Render:
				return new RenderComponent();
			case Type::Light:
				return new LightComponent();
			case Type::Terrain:
				return new TerrainComponent();
		}
		return nullptr;
	}
}