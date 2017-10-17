#ifndef SHADOW_GLES20_STATE_DIFFERENCE_INCLUDE
#define SHADOW_GLES20_STATE_DIFFERENCE_INCLUDE

#include "../../../Globals.h"

namespace sh
{
	namespace video
	{
		class GLES20StateDifference
		{
		public:
			GLES20StateDifference(const MaterialPtr& previousMaterial, const MaterialPtr& currentMaterial);
			void Apply();

		private:
			std::vector<std::function<void()>> m_functions;
		};
	}
}

#endif
