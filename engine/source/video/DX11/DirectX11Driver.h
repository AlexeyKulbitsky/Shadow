#ifndef SHADOW_DIRECTX_11_DRIVER_INCLUDE
#define SHADOW_DIRECTX_11_DRIVER_INCLUDE

#include "../Driver.h"
#include "DirectX11Common.h"

namespace sh
{

namespace video
{

	class DirectX11Driver : public Driver
	{
		DirectX11Driver(){}
		~DirectX11Driver(){}

		virtual const String& GetApiName() const override;

		virtual bool Init() override;
		virtual void BeginRendering() override;
		virtual void EndRendering() override;
			

		virtual VertexInputDeclarationPtr CreateVertexInputDeclaration() const override { return nullptr; }
	};

} // video

} // sh

#endif