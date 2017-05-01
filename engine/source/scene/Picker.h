#ifndef SHADOW_PICKER_INCLUDE
#define SHADOW_PICKER_INCLUDE

#include "../Globals.h"
#include "../video/GpuParams.h"

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

			sh::video::MaterialPtr m_material;
			sh::video::GpuParamsPtr m_params;
			sh::video::GpuParamMatrix4f m_wvpMtrix;
			sh::video::GpuParamVector4f m_color;
		};
	}
}

#endif