#include "DirectX11Driver.h"

namespace sh
{

namespace video
{

	const String& DirectX11Driver::GetApiName() const
	{
		static const String s_apiName = "DirectX11";
		return s_apiName;
	}

	bool DirectX11Driver::Init()
	{
		return true;
	}

	void DirectX11Driver::BeginRendering()
	{

	}

	void DirectX11Driver::EndRendering()
	{

	}

} // video

} // sh