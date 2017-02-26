#ifndef SHADOW_PICKER_INCLUDE
#define SHADOW_PICKER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class Picker
		{
		public:
			Picker();
			~Picker();
			void RegisterEntity(sh::Entity* entity);
			sh::Entity* TryToPick(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height);
			void Clear();

		private:
			std::vector<sh::Entity*> m_entities;
			sh::video::RenderTechniquePtr m_renderTechnique = nullptr;
		};
	}
}

#endif