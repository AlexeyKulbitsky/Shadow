#include "VertexDeclaration.h"
#include "Managers/RenderStateManager.h"

namespace sh
{

namespace video
{

	VertexInputDeclarationPtr VertexInputDeclaration::Create()
	{
		return RenderStateManager::GetInstance()->CreateVertexDeclaration();
	}

} // video

} // sh