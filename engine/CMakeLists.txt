cmake_minimum_required(VERSION 3.4.1)

project (ShadowEngine)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0015 NEW)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/source)
set(PROJECT_NAME Shadow)

include_directories( ${PROJECT_INCLUDE_DIR} )

file(GLOB_RECURSE PROJECT_SOURCE_FILES "*.h" "*.hpp" "*.cpp")

#-----------------------------------------

macro(GroupSources curdir)

file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
${PROJECT_SOURCE_DIR}/${curdir}/*)

    foreach(child ${children})

           if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})

              GroupSources(${curdir}/${child})

           else()

              string(REPLACE "/" "\\" groupname ${curdir})
              string(REPLACE "src" "Common" groupname ${groupname})
              source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})

           endif()

    endforeach()

endmacro()

#-----------------------------------------

GroupSources(source)

# Add DirectX
if (WIN32)
  include_directories($ENV{DXSDK_DIR}/Include)
endif(WIN32)

# Add pugixml
add_subdirectory(../libs/pugixml "${CMAKE_CURRENT_BINARY_DIR}/pugixml_build")
include_directories( ../libs/pugixml/src )

set_property(TARGET pugixml PROPERTY FOLDER CommonLibs)

# Add freetype
add_subdirectory(../libs/freetype "${CMAKE_CURRENT_BINARY_DIR}/freetype_build")
include_directories(../libs/freetype/include)

set_property(TARGET freetype PROPERTY FOLDER CommonLibs)

# Add Vulkan
include_directories(../libs/vulkan/include)

# Add stb
include_directories(../libs/stb)

# Add Tiny OBJ loader
include_directories(../libs/tinyobjloader)

# Add EGL
include_directories(../libs/egl/include)
find_library(EGL_LIBRARY libEGL HINTS ../libs/egl/lib/release)
find_library(GLESV2_LIBRARY libGLESv2 HINTS ../libs/egl/lib/release)

# Add glslang library
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

# Add assimp
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built in addition to the library." FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library." FORCE)

add_subdirectory(../libs/assimp "${CMAKE_CURRENT_BINARY_DIR}/assimp_build")
include_directories(../libs/assimp/include)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/assimp_build/include")

set_property(TARGET assimp PROPERTY FOLDER CommonLibs)
set_property(TARGET IrrXML PROPERTY FOLDER CommonLibs)
set_property(TARGET zlibstatic PROPERTY FOLDER CommonLibs)
set_property(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTY FOLDER CommonLibs)
set_property(TARGET uninstall PROPERTY FOLDER CommonLibs)
set_property(TARGET dist PROPERTY FOLDER CommonLibs)

add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} pugixml freetype glslang SPIRV SPVRemapper assimp ${EGL_LIBRARY} ${GLESV2_LIBRARY})
