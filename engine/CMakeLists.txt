cmake_minimum_required(VERSION 3.4.1)

project (ShadowEngine)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0015 NEW)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/source)
set(PROJECT_NAME Shadow)

#set(USE_VULKAN_API OFF)

include_directories( ${PROJECT_INCLUDE_DIR} )

file(GLOB_RECURSE PROJECT_SOURCE_FILES "source/*.h" "source/*.hpp" "source/*.cpp")

# exclude platform-specific files
if (SHADOW_ANDROID)

	set (EXCLUDE_DIR "/win32/")
	foreach (TMP_PATH ${PROJECT_SOURCE_FILES})
		string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
		if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
			list (REMOVE_ITEM PROJECT_SOURCE_FILES ${TMP_PATH})
		endif ()
	endforeach(TMP_PATH)
	
else (SHADOW_ANDROID)

	set (EXCLUDE_DIR "/android/")
	foreach (TMP_PATH ${PROJECT_SOURCE_FILES})
		string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
		if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
			list (REMOVE_ITEM PROJECT_SOURCE_FILES ${TMP_PATH})
		endif ()
	endforeach(TMP_PATH)
	
endif(SHADOW_ANDROID)

# Exclude Vulkan API if flag is set
if (NOT USE_VULKAN_API)

  set (EXCLUDE_VULKAN_DIR "/Vulkan/")
  foreach (TMP_PATH ${PROJECT_SOURCE_FILES})
    string (FIND ${TMP_PATH} ${EXCLUDE_VULKAN_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
      list (REMOVE_ITEM PROJECT_SOURCE_FILES ${TMP_PATH})
    endif ()
  endforeach(TMP_PATH)

else()

  add_definitions(-DSHADOW_USE_VULKAN_API)

endif()

# Group files according to files organization
foreach(FILE ${PROJECT_SOURCE_FILES}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

#-----------------------------------------

macro(GroupAllSources curdir)

	foreach(child ${curdir})
		if(IS_DIRECTORY ${curdir}/${child})
			GroupAllSources(${curdir}/${child})
		else()
			string(REPLACE "/" "\\" groupname ${curdir})
            string(REPLACE "src" "Common" groupname ${groupname})
            source_group(${groupname} FILES ${curdir}/${child})
		endif()
	endforeach()

endmacro()

#-----------------------------------------

# Add DirectX
if (WIN32)
  include_directories($ENV{DXSDK_DIR}/Include)
endif(WIN32)

# Add Shadow engine library
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

# Add pugixml
add_subdirectory(../libs/pugixml "${CMAKE_CURRENT_BINARY_DIR}/pugixml_build")
include_directories( ../libs/pugixml/src )

set_property(TARGET pugixml PROPERTY FOLDER CommonLibs)

# Add freetype
add_subdirectory(../libs/freetype "${CMAKE_CURRENT_BINARY_DIR}/freetype_build")
include_directories(../libs/freetype/include)

set_property(TARGET freetype PROPERTY FOLDER CommonLibs)

# Add Vulkan
if (USE_VULKAN_API)
  include_directories(../libs/vulkan/include)

  set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "Builds glslangValidator and spirv-remap" FORCE)
  set(ENABLE_HLSL OFF CACHE BOOL "Enables HLSL input support" FORCE)

  add_subdirectory(../libs/glslang "${CMAKE_CURRENT_BINARY_DIR}/glslang_build")
  include_directories(../libs/glslang)
  include_directories(../libs/glslang/StandAlone)

  set_property(TARGET glslang PROPERTY FOLDER CommonLibs)
  set_property(TARGET SPIRV PROPERTY FOLDER CommonLibs)
  set_property(TARGET SPVRemapper PROPERTY FOLDER CommonLibs)
  set_property(TARGET OGLCompiler PROPERTY FOLDER CommonLibs)
  set_property(TARGET OSDependent PROPERTY FOLDER CommonLibs)

  target_link_libraries(${PROJECT_NAME} glslang SPIRV SPVRemapper)
endif()

# Add stb
include_directories(../libs/stb)

# Add Tiny OBJ loader
include_directories(../libs/tinyobjloader)

# Add EGL
include_directories(../libs/egl/include)
find_library(EGL_LIBRARY libEGL HINTS ../libs/egl/lib/release)
find_library(GLESV2_LIBRARY libGLESv2 HINTS ../libs/egl/lib/release)

# Add GLEW
if (WIN32)
  set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
  add_subdirectory(../libs/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/glew_build")
  include_directories(../libs/glew/include)
  target_link_libraries(${PROJECT_NAME} glew_s)

  if(MSVC12 OR MSVC14)
    set_property(TARGET glew_s PROPERTY FOLDER CommonLibs)
    set_property(TARGET glew PROPERTY FOLDER CommonLibs)
  endif(MSVC12 OR MSVC14)

endif(WIN32)

# Add assimp
if (USE_ASSIMP_API)
  add_definitions(-DSHADOW_ASSIMP_LOADER)

  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built in addition to the library." FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library." FORCE)
  add_subdirectory(../libs/assimp "${CMAKE_CURRENT_BINARY_DIR}/assimp_build")
  include_directories(../libs/assimp/include)
  include_directories("${CMAKE_CURRENT_BINARY_DIR}/assimp_build/include")
  target_link_libraries(${PROJECT_NAME} assimp)

  if(MSVC12 OR MSVC14)
    set_property(TARGET assimp PROPERTY FOLDER CommonLibs)
    set_property(TARGET IrrXML PROPERTY FOLDER CommonLibs)
    set_property(TARGET zlibstatic PROPERTY FOLDER CommonLibs)
    set_property(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTY FOLDER CommonLibs)
    set_property(TARGET uninstall PROPERTY FOLDER CommonLibs)
    
  endif(MSVC12 OR MSVC14)
endif()

set_property(TARGET dist PROPERTY FOLDER CommonLibs)

if (SHADOW_ANDROID)
  add_subdirectory(../libs/minizip "${CMAKE_CURRENT_BINARY_DIR}/minizip_build")
  include_directories(../libs/minizip)

  target_link_libraries(${PROJECT_NAME} pugixml freetype minizip)
else(SHADOW_ANDROID)
  target_link_libraries(${PROJECT_NAME} pugixml freetype)# ${EGL_LIBRARY} ${GLESV2_LIBRARY})
endif(SHADOW_ANDROID)

