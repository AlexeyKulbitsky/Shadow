cmake_minimum_required(VERSION 3.4.1)

project (Framework)

# Add engine
add_subdirectory(../engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
include_directories(../engine/source)

include_directories( ../libs/pugixml/src )
include_directories(../libs/freetype/include)
include_directories(../libs/assimp/include)

file(GLOB FRAMEWORK_SRC "source/*.cpp")

if (SHADOW_ANDROID)
	add_library(framework SHARED

             ${FRAMEWORK_SRC})
             # source/platform/android/ShadowJNI.cpp)

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
                android
                GLESv2
                EGL)

else (SHADOW_ANDROID)
	add_executable(framework ${FRAMEWORK_SRC})
	target_link_libraries(framework Shadow)
endif (SHADOW_ANDROID)





if (SHADOW_WINDOWS)
add_custom_command(TARGET framework POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E 
					copy ${CMAKE_CURRENT_BINARY_DIR}/../../libs/egl/bin/release/libEGL.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>
					COMMAND ${CMAKE_COMMAND} -E 
					copy ${CMAKE_CURRENT_BINARY_DIR}/../../libs/egl/bin/release/libGLESv2.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>)

endif (SHADOW_WINDOWS)
