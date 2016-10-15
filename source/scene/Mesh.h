#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include "../video/HardwareBuffer.h"

namespace sh
{
	namespace scene
	{		
		class Mesh
		{
		public:
			Mesh() {}
			
		private:
			video::HardwareBuffer m_buffer;
		};
	}
}
#endif