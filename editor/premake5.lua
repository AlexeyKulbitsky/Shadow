--require( "qt" )
--local qt = premake.extensions.qt

-- main solution
solution "Shadow Editor"
	configurations { "Debug", "Release" }
	platforms { "Win32", "Win64" }
	location "prj"
	--
	-- setup your solution's configuration here ...
	--
	
	include "../engine/premake5.lua"		

	-- main project
	project "Editor"
		kind "ConsoleApp"
		language "C++"
		includedirs { 
			"source/**",
			"../libs/pugixml/src/",
			"../libs/freetype/include/",
			"../engine/source/"
		}
		
		files { 
			"source/**", 
		}
		
		filter "configurations:Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			
		filter "platforms:Win32"
			libdirs { 
				"../libs/egl/lib/release", 
				"../libs/vulkan/bin", 
				"../libs/freetype/lib/release"
				--"c:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/" 
				}
			links { 
			"libEGL", 
			"libGLESv2", 
			"vulkan-1",
			--"dxgi",
			--"d3d11",
			--"d3dx11",
			--"d3dx10",
			"freetype271",
			"Shadow" }
			characterset ("MBCS") -- Multi-byte Character Set; currently Visual Studio only
			characterset ("Default") --the default encoding for the toolset; usually Unicode
			characterset ("MBCS") --Unicode: Unicode character encoding

			postbuildcommands {
				"{COPY} ../../libs/egl/bin/release/libEGL.dll ../prj/bin/%{cfg.platform}/%{cfg.buildcfg}/",
				"{COPY} ../../libs/egl/bin/release/libGLESv2.dll ../prj/bin/%{cfg.platform}/%{cfg.buildcfg}/",
				"{COPY} ../../libs/vulkan/bin/vulkan-1.dll ../prj/bin/%{cfg.platform}/%{cfg.buildcfg}/",
			}