#ifndef SHADOW_WIN32_DEVICE_INCLUDE
#define SHADOW_WIN32_DEVICE_INCLUDE

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h> // For JOYCAPS
#include <windowsx.h>

#include "../../Device.h"

namespace sh
{
	class Win32Device : public Device 
	{
	public:
		Win32Device();
		explicit Win32Device(const CreationParameters &parameters);
		virtual ~Win32Device();

		void Init() override final;
		void Update(float deltaTime) override final;
		void Run() override final;
		uint64_t GetTime() override final;

	private:
		HWND m_hwnd;
	};
}



#endif
