#include "ComponentsFactory.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/LightComponent.h"
#include "components/TerrainComponent.h"

namespace sh
{
	//////////////////////////////////////////////////////////////////////////////////////////

	ComponentsFactory::~ComponentsFactory()
	{
		for (size_t i = 0; i < m_components.size(); ++i)
		{
			delete m_components[i];
			m_components[i] = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////

}