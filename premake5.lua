-- premake5.lua
workspace "Shadow"
   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64" }
   location "prj"

project "Shadow"
   kind "ConsoleApp"
   language "C++"
   includedirs { 
		"source/**",
		"libs/pugixml/src/",
		"libs/egl/include/"
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
		libdirs { "libs/egl/lib" }
		links { "libEGL", "libGLESv2" }
		characterset ("MBCS") -- Multi-byte Character Set; currently Visual Studio only
--		characterset ("Default") --the default encoding for the toolset; usually Unicode
--		characterset ("MBCS") --Unicode: Unicode character encoding

		-- copy a file from the objects directory to the target directory
		postbuildcommands {
			"{COPY} ../libs/egl/lib/libEGL.dll ../bin/%{cfg.buildcfg}/",
			"{COPY} ../libs/egl/lib/libGLESv2.dll ../bin/%{cfg.buildcfg}/",
			}
		
	filter "platforms:Win64"
		libdirs { "libs/egl/lib" }
		links { "libEGL", "libGLESv2" }
		characterset ("MBCS")
	  
--	filter { "platforms:Static" }
--		kind "StaticLib"

--	filter { "platforms:DLL" }
--		kind "SharedLib"
--		defines { "DLL_EXPORTS" }