cmake_minimum_required(VERSION 3.4.1)

project (Framework)

# Add engine
set(USE_VULKAN_API OFF)
add_subdirectory(../engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
include_directories(../engine/source)

include_directories( ../libs/pugixml/src )
include_directories(../libs/freetype/include)

if(USE_ASSIMP_API)
  include_directories(../libs/assimp/include)
endif()

file(GLOB_RECURSE FRAMEWORK_SRC "source/*.h" "source/*.hpp" "source/*.cpp")

if (SHADOW_ANDROID)

  set (EXCLUDE_DIR "/win32/")
  foreach (TMP_PATH ${FRAMEWORK_SRC})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
      list (REMOVE_ITEM FRAMEWORK_SRC ${TMP_PATH})
    endif ()
  endforeach(TMP_PATH)

	add_library(framework SHARED

             ${FRAMEWORK_SRC})

    find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

    target_link_libraries(framework
    			Shadow
    			# Links the target library to the log library
                # included in the NDK.
                ${log-lib}
                z
                android
                GLESv2
                EGL)

else (SHADOW_ANDROID)

  set (EXCLUDE_DIR "/android/")
  foreach (TMP_PATH ${FRAMEWORK_SRC})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
      list (REMOVE_ITEM FRAMEWORK_SRC ${TMP_PATH})
    endif ()
  endforeach(TMP_PATH)

  # Group files according to files organization
  foreach(FILE ${FRAMEWORK_SRC}) 
      # Get the directory of the source file
      get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

      # Remove common directory prefix to make the group
      string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

      # Make sure we are using windows slashes
      string(REPLACE "/" "\\" GROUP "${GROUP}")

      source_group("${GROUP}" FILES "${FILE}")
  endforeach()
  
	add_executable(framework ${FRAMEWORK_SRC})
	target_link_libraries(framework Shadow)

endif (SHADOW_ANDROID)





if (WIN32)
add_custom_command(TARGET framework POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E 
					copy ${CMAKE_CURRENT_BINARY_DIR}/../../libs/egl/bin/release/libEGL.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>
					COMMAND ${CMAKE_COMMAND} -E 
					copy ${CMAKE_CURRENT_BINARY_DIR}/../../libs/egl/bin/release/libGLESv2.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>)

endif (WIN32)
