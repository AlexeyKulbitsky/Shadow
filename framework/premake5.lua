workspace "Shadow Framework"
   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64" }
   location "prj"
   
include "../libs/freetype/premake5.lua"   
include "../engine/premake5.lua"

project "Application"
   kind "ConsoleApp"
   language "C++"
   includedirs { 
		"source/**",
		"../libs/pugixml/src/",
		"../libs/freetype/include/",
		"../engine/source/"
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
		libdirs { "../libs/egl/lib/release", "../libs/freetype/lib/release" }
		links { "libEGL", "libGLESv2", "Freetype", "Shadow" }
		characterset ("MBCS") -- Multi-byte Character Set; currently Visual Studio only
--		characterset ("Default") --the default encoding for the toolset; usually Unicode
--		characterset ("MBCS") --Unicode: Unicode character encoding

		postbuildcommands {
			"{COPY} ../../libs/egl/bin/release/libEGL.dll ../bin/%{cfg.buildcfg}/",
			"{COPY} ../../libs/egl/bin/release/libGLESv2.dll ../bin/%{cfg.buildcfg}/",
			}

		
	filter "platforms:Win64"
		characterset ("MBCS")