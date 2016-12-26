#include "Component.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/LightComponent.h"

namespace sh
{
	Component* Component::Create(Component::Type type)
	{
		switch (type)
		{
			case Type::TRANSFORM:
				return new TransformComponent();
			case Type::RENDER:
				return new RenderComponent();
			case Type::LIGHT:
				return new LightComponent();
		}
		return nullptr;
	}
}