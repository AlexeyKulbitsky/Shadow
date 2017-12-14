#include "Component.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/LightComponent.h"
#include "components/TerrainComponent.h"

namespace sh
{
	Component* Component::Clone()
	{
		return nullptr;
	}
}