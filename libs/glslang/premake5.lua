workspace "Test Framework"
   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64" }
   location "prj"
   

include "OGLCompilersDLL/premake5.lua"
include "SPIRV/premake5.lua"  
include "glslang/premake5.lua" 

project "TestApplication"
   kind "ConsoleApp"
   language "C++"
   includedirs { 
      "../",
		"source/**",
	}
   
   targetdir "bin/%{cfg.buildcfg}"
   objdir "prj/obj/%{cfg.buildcfg}"

   files { 
		"source/**", 
		}

   filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
	  
	filter "platforms:Win32"
		links { "glslang", "SPIRV", "SPVRemapper", "OGLCompiler", "OSDependent" }

		
	filter "platforms:Win64"
		characterset ("MBCS")