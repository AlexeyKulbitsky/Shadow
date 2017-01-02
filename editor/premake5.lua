require( "qt" )
local qt = premake.extensions.qt

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
			"../engine/source/"
		}
		--
		-- setup your project's configuration here ...
		--

		-- add the files
		files { "**.h", "**.cpp", "**.ui", "**.qrc" }


		--
		-- Enable Qt for this project.
		--
		qt.enable()

		--
		-- Setup the Qt path. This apply to the current configuration, so
		-- if you handle x32 and x64, you can specify a different path
		-- for both configurations.
		--
		-- Note that if this is ommited, the addon will try to look for the
		-- QTDIR environment variable. If it's not found, then the script
		-- will return an error since it won't be able to find the path
		-- to your Qt installation.
		--
		qtpath "D:/Qt_5_XXX/5.6/msvc2013"

		--
		-- Setup which Qt modules will be used. This also apply to the
		-- current configuration, so can you choose to deactivate a module
		-- for a specific configuration.
		--
		qtmodules { "core", "gui", "widgets", "opengl" }

		--
		-- Setup the prefix of the Qt libraries. Usually it's Qt4 for Qt 4.x
		-- versions and Qt5 for Qt 5.x ones. Again, this apply to the current
		-- configuration. So if you want to have a configuration which uses
		-- Qt4 and one that uses Qt5, you can do it.
		--
		qtprefix "Qt5"

		--
		-- Setup the suffix for the Qt libraries. The debug versions of the
		-- Qt libraries usually have a "d" suffix. If you compiled your own
		-- version, you could also have suffixes for x64 libraries, etc.
		--
		filter "configurations:Debug"
			qtsuffix "d"
			defines { "DEBUG" }
			flags { "Symbols" }
			
		filter "platforms:Win32"
			libdirs { "../libs/egl/lib", "../libs/vulkan/bin" }
			links { "libEGL", "libGLESv2", "vulkan-1", "Shadow" }
			characterset ("MBCS") -- Multi-byte Character Set; currently Visual Studio only
--			characterset ("Default") --the default encoding for the toolset; usually Unicode
--			characterset ("MBCS") --Unicode: Unicode character encoding

			postbuildcommands {
				"{COPY} ../../libs/egl/lib/libEGL.dll ../prj/bin/%{cfg.platform}/%{cfg.buildcfg}/",
				"{COPY} ../../libs/egl/lib/libGLESv2.dll ../prj/bin/%{cfg.platform}/%{cfg.buildcfg}/",
				"{COPY} ../../libs/vulkan/bin/vulkan-1.dll ../prj/bin/%{cfg.platform}/%{cfg.buildcfg}/",
			}