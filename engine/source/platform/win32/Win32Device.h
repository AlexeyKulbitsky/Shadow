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

		virtual void Init() override;
		virtual void Update(f32 deltaTime) override;
		virtual void Run() override;
		virtual u64 GetTime() override;

        String ShowOpenFileDialog() override final;
        String ShowSaveFileDialog() override final;

		bool CreateDriver();
	private:
		HWND m_hwnd;
	};
}



#endif
