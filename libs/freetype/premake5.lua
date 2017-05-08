project "Freetype"
   kind "StaticLib"
   language "C++"
   includedirs 
   	{ 
		"include"
	}
   
   targetdir "lib/%{cfg.buildcfg}"
   objdir "prj/obj/%{cfg.buildcfg}"

   files
    {
        "src/autofit/autofit.c",
        "src/bdf/bdf.c",
        "src/cff/cff.c",
        "src/base/ftbase.c",
        "src/base/ftbitmap.c",
        "src/cache/ftcache.c",
        "windows/ftdebug.c", --WINDOWS SPECIFIC INCLUDE
        "src/base/ftfstype.c",
        "src/base/ftgasp.c",
        "src/base/ftglyph.c",
        "src/gzip/ftgzip.c",
        "src/base/ftinit.c",
        "src/lzw/ftlzw.c",
        "src/base/ftstroke.c",
        "src/base/ftsystem.c",
        "src/smooth/smooth.c",

        "src/base/ftbbox.c",
        "src/base/ftfntfmt.c",
        "src/base/ftgxval.c",
        "src/base/ftlcdfil.c",
        "src/base/ftmm.c",
        "src/base/ftotval.c",
        "src/base/ftpatent.c",
        "src/base/ftpfr.c",
        "src/base/ftsynth.c",
        "src/base/fttype1.c",
        "src/base/ftwinfnt.c",
 --       "src/base/ftxf86.c",
 --       "src/base/ftfstype.c",
        "src/pcf/pcf.c",
        "src/pfr/pfr.c",
        "src/psaux/psaux.c",
        "src/pshinter/pshinter.c",
        "src/psnames/psmodule.c",
        "src/raster/raster.c",
        "src/sfnt/sfnt.c",
        "src/truetype/truetype.c",
        "src/type1/type1.c",
        "src/cid/type1cid.c",
        "src/type42/type42.c",
        "src/winfonts/winfnt.c",      
    }

    defines
    {
        "WIN32",
        "_CRT_SECURE_NO_WARNINGS",
        "FT2_BUILD_LIBRARY",      
    }  

   filter "configurations:Debug"
      defines { 
      		"_DEBUG", 
      		"FT_DEBUG_LEVEL_ERROR",
            "FT_DEBUG_LEVEL_TRACE"
 			}
      flags { "Symbols" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
	  
	filter "platforms:Win32"
		characterset ("MBCS") -- Multi-byte Character Set; currently Visual Studio only

	filter "platforms:Win64"
		characterset ("MBCS")
