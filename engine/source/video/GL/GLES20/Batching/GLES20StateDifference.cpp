#include "GLES20StateDifference.h"

#include "../../../Material.h"

namespace sh
{
	namespace video
	{
		GLES20StateDifference::GLES20StateDifference(const MaterialPtr& previousMaterial,
													 const MaterialPtr& currentMaterial)
		{

			previousMaterial->GetRenderPipeline();

		}

		///////////////////////////////////////////////////////////////////////

		void GLES20StateDifference::Apply()
		{

		}
	}
}