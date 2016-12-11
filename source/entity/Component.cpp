#include "Component.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"

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
		}
		return nullptr;
	}
}