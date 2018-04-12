#ifndef SHADOW_DYNAMIC_LIBRARY_INCLUDE
#define SHADOW_DYNAMIC_LIBRARY_INCLUDE

#include "Globals.h"

#if defined SHADOW_WINDOWS
struct HINSTANCE__;
//typedef struct HINSTANCE__* hInstance;
using LibraryHandle = struct HINSTANCE__*;
#else
using LibraryHandle = void*;
#endif

namespace sh
{

	class SHADOW_API DynamicLibrary
	{
	public:
		DynamicLibrary(const String& filename);
		bool Load();
		bool Unload();

		void* GetSymbol(const String& name);

	private:
		String m_filename;
		LibraryHandle m_handle;
	};

} // sh

#endif
