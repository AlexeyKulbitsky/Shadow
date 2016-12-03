#ifndef SHADOW_ENTITY_INCLUDE
#define SHADOW_ENTITY_INCLUDE

#include "../Globals.h"
#include "Model.h"
#include "SubEntity.h"

namespace sh
{
	namespace scene
	{
		class Entity
		{
		public:
			explicit Entity(Model* model);
			void Render();

		private:
			Entity(){}
			Entity(const Entity* other){}
			
		private:
			std::vector<SubEntity*> m_subEntities;
			Model* m_model;
		};
	}
}

#endif