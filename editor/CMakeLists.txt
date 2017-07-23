cmake_minimum_required(VERSION 3.4.1)

project (Editor)

# Add engine
add_subdirectory(../engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
include_directories(../engine/source)

include_directories( ../libs/pugixml/src )
include_directories(../libs/freetype/include)

if(USE_ASSIMP_API)
  include_directories(../libs/assimp/include)
endif()


set(EDITOR_PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/source)
set(EDITOR_PROJECT_NAME Editor)

include_directories( ${EDITOR_PROJECT_INCLUDE_DIR} )

file(GLOB_RECURSE EDITOR_PROJECT_SOURCE_FILES "source/*.h" "source/*.hpp" "source/*.cpp")

# Group files according to files organization
foreach(FILE ${EDITOR_PROJECT_SOURCE_FILES}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

add_executable(${EDITOR_PROJECT_NAME} ${EDITOR_PROJECT_SOURCE_FILES})

target_link_libraries(${EDITOR_PROJECT_NAME} Shadow)

add_custom_command(TARGET ${EDITOR_PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E 
					copy ${CMAKE_CURRENT_BINARY_DIR}/../../libs/egl/bin/release/libEGL.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>
					COMMAND ${CMAKE_COMMAND} -E 
					copy ${CMAKE_CURRENT_BINARY_DIR}/../../libs/egl/bin/release/libGLESv2.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>)
