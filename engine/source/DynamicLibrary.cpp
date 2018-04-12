#include "DynamicLibrary.h"

#if defined(SHADOW_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#if !defined(NOMINMAX) && defined(_MSC_VER)
		#define NOMINMAX // required to stop windows.h messing up std::min
	#endif
	#include <windows.h>

#elif defined(SHADOW_ANDROID) || defined(SHADOW_MAC)
    #include <dlfcn.h>
#endif

namespace sh
{

	DynamicLibrary::DynamicLibrary(const String& filename)
		: m_filename(filename)
	{
	}

	bool DynamicLibrary::Load()
	{
#if defined(SHADOW_WINDOWS)
		m_handle = LoadLibrary(m_filename.c_str());
#elif defined(SHADOW_ANDROID) || defined(SHADOW_MAC)
		m_handle = dlopen(m_filename.c_str(), RTLD_NOW | RTLD_LOCAL);
        //m_handle = dlopen(m_filename.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif
		return m_handle != nullptr;
	}

	bool DynamicLibrary::Unload()
	{
#if defined(SHADOW_WINDOWS)
		return FreeLibrary(m_handle);
#elif defined(SHADOW_ANDROID) || defined(SHADOW_MAC)
		return dlclose(m_handle);
#endif
	}

	void* DynamicLibrary::GetSymbol(const String& name)
	{
		if (!m_handle)
			return nullptr;

#if defined(SHADOW_WINDOWS)
		return GetProcAddress(m_handle, name.c_str());
#elif defined(SHADOW_ANDROID) || defined(SHADOW_MAC)
		return dlsym(m_handle, name.c_str());
#endif
	}


} // sh
