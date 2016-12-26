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
			
	project "Shadow"
   kind "ConsoleApp"
   language "C++"
   includedirs { 
		"../engine/source/**",
		"../libs/pugixml/src/",
		"../libs/egl/include/",
		"../libs/soil/include/",
		"../libs/vulkan/include/",
		"../libs/stb/include/",
		"../libs/tinyobjloader/include/"
	}
   
   targetdir "bin/%{cfg.buildcfg}"
   objdir "prj/obj/%{cfg.buildcfg}"

   files { 
		"../engine/source/**", 
		}

   filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
	  
	filter "platforms:Win32"
		libdirs { "../libs/egl/lib", "../libs/soil/lib/debug", "../libs/vulkan/bin" }
		links { "libEGL", "libGLESv2", "SOIL", "vulkan-1" }
		characterset ("MBCS") -- Multi-byte Character Set; currently Visual Studio only
--		characterset ("Default") --the default encoding for the toolset; usually Unicode
--		characterset ("MBCS") --Unicode: Unicode character encoding

		-- copy a file from the objects directory to the target directory
		postbuildcommands {
			"{COPY} ../../libs/egl/lib/libEGL.dll ../bin/%{cfg.buildcfg}/",
			"{COPY} ../../libs/egl/lib/libGLESv2.dll ../bin/%{cfg.buildcfg}/",
			"{COPY} ../../libs/vulkan/bin/vulkan-1.dll ../bin/%{cfg.buildcfg}/",
			}
		
	filter "platforms:Win64"
		libdirs { "../libs/egl/lib", "../libs/soil/lib/debug", "../libs/vulkan/bin" }
		links { "libEGL", "libGLESv2" }
		characterset ("MBCS")
	
	
	

	-- main project
	project "Shadow Editor"
		kind "ConsoleApp"
		language "C++"
		includedirs { 
		"source/**",
		"../engine/source/",
		"../libs/pugixml/src/",
		"../libs/egl/include/",
		"../libs/soil/include/",
		"../libs/vulkan/include/",
		"../libs/stb/include/",
		"../libs/tinyobjloader/include/"
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
		--qtpath "D:/Qt/5.6"

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