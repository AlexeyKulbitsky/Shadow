include "OSDependent/Windows/premake5.lua"

project "glslang"
   kind "StaticLib"
   language "C++"
   includedirs 
   	{ 
		"GenericCodeGen/", 
		"Include/",
		"MachineIndependent/",
		"Public/"
	}
   
   targetdir "lib/%{cfg.buildcfg}"
   objdir "prj/obj/%{cfg.buildcfg}"

   files { 
		"GenericCodeGen/**", 
		"Include/**",
		"MachineIndependent/**",
		"Public/**"
		}

   filter "configurations:Debug"
      defines { "_DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
	  
	filter "platforms:Win32"
		characterset ("MBCS") 

	filter "platforms:Win64"
		characterset ("MBCS")
