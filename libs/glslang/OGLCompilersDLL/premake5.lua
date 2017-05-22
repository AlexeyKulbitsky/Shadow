project "OGLCompiler"
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
        "InitializeDll.cpp"     
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
