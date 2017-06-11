cmake_minimum_required(VERSION 3.4.1)

project (Editor)

# Add engine
add_subdirectory(../engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
include_directories(../engine/source)

include_directories( ../libs/pugixml/src )
include_directories(../libs/freetype/include)
include_directories(../libs/assimp/include)


set(EDITOR_PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/source)
set(EDITOR_PROJECT_NAME Editor)

include_directories( ${EDITOR_PROJECT_INCLUDE_DIR} )

file(GLOB_RECURSE EDITOR_PROJECT_SOURCE_FILES "*.h" "*.hpp" "*.cpp")

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
add_executable(${EDITOR_PROJECT_NAME} ${EDITOR_PROJECT_SOURCE_FILES})

target_link_libraries(${EDITOR_PROJECT_NAME} Shadow)

add_custom_command(TARGET ${EDITOR_PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E 
					copy ${CMAKE_CURRENT_BINARY_DIR}/../../libs/egl/bin/release/libEGL.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>
					COMMAND ${CMAKE_COMMAND} -E 
					copy ${CMAKE_CURRENT_BINARY_DIR}/../../libs/egl/bin/release/libGLESv2.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>)
