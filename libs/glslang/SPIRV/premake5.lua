project "SPIRV"
   kind "StaticLib"
   language "C++"
   includedirs 
   	{ 
		""
	}
   
   targetdir "lib/%{cfg.buildcfg}"
   objdir "prj/obj/%{cfg.buildcfg}"

   files
    {      
		"GlslangToSpv.cpp",
		"InReadableOrder.cpp",
		"Logger.cpp",
		"SpvBuilder.cpp",
		"doc.cpp",
		"disassemble.cpp"     
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
		
		
project "SPVRemapper"
   kind "StaticLib"
   language "C++"
   includedirs 
   	{ 
		""
	}
   
   targetdir "lib/%{cfg.buildcfg}"
   objdir "prj/obj/%{cfg.buildcfg}"

   files
    {      
		"SPVRemapper.cpp",
		"doc.cpp"     
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
		