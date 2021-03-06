cmake_minimum_required(VERSION 3.4.1)

project (Game)

set(USE_ASSIMP_API ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../lib")

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")

# Add engine (must be compolied before linking)
#add_subdirectory(../../../engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
find_library(SHADOW_LIBRARY Shadow HINTS ../../../engine/lib/Debug)

if( SHADOW_LIBRARY STREQUAL SHADOW_LIBRARY-NOTFOUND)
    MESSAGE(FATAL_ERROR "Framework SHADOW_LIBRARY not found")
else()
	MESSAGE(STATUS "Found engine")
endif()


include_directories(../../../engine/source)

#include_directories( ../../../libs/pugixml/src )
#include_directories(../../../libs/freetype/include)

#if(USE_ASSIMP_API)
#  include_directories(../../../libs/assimp/include)
#endif()


set(GAME_PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/source)
set(GAME_LIBRARY_NAME GameLibrary)
set(GAME_EXECUTABLE_NAME GameExecutable)

include_directories( ${GAME_PROJECT_INCLUDE_DIR} )

file(GLOB_RECURSE GAME_PROJECT_SOURCE_FILES "*.h" "*.hpp" "*.cpp")

# Group files according to files organization
foreach(FILE ${GAME_PROJECT_SOURCE_FILES}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# Set object files for compiling both executable and dll version of game
#add_library(GameCommon OBJECT ${GAME_PROJECT_SOURCE_FILES})
#set_property(TARGET ${GameCommon} PROPERTY POSITION_INDEPENDENT_CODE 1)


# Add executable and dll version
#add_library(${GAME_LIBRARY_NAME} SHARED $<TARGET_OBJECTS:GameCommon>)
#add_executable(${GAME_EXECUTABLE_NAME} $<TARGET_OBJECTS:GameCommon>)

add_library(${GAME_LIBRARY_NAME} SHARED ${GAME_PROJECT_SOURCE_FILES})
add_executable(${GAME_EXECUTABLE_NAME} MACOSX_BUNDLE ${GAME_PROJECT_SOURCE_FILES})

target_link_libraries(${GAME_LIBRARY_NAME} ${SHADOW_LIBRARY})
target_link_libraries(${GAME_EXECUTABLE_NAME} ${SHADOW_LIBRARY})

if (WIN32)
add_custom_command(TARGET ${GAME_EXECUTABLE_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E 
                    copy ../../../engine/bin/$<CONFIGURATION>/Shadow.dll ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)
endif()

