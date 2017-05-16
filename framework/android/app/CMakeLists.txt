# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.


# Sets freetype_src_DIR to the path of the target CMake project.
set( freetype_src_DIR ../../../libs/freetype )

# Sets lib_build_DIR to the path of the desired output directory.
set( freetype_build_DIR ../../../libs/freetype/lib/testOutput )
file(MAKE_DIRECTORY ${freetype_build_DIR})

# Adds the CMakeLists.txt file located in the specified directory
# as a build dependency.
add_subdirectory( # Specifies the directory of the CMakeLists.txt file.
                  ${freetype_src_DIR}

                  # Specifies the directory for the build outputs.
                  ${freetype_build_DIR} )

# Adds the output of the additional CMake build as a prebuilt static
# library and names it lib_gmath.
#add_library( lib_freetype STATIC IMPORTED )
set_target_properties( lib-freetype PROPERTIES IMPORTED_LOCATION
                       ${freetype_build_DIR}/${ANDROID_ABI}/lib-freetype.a )
include_directories( ${freetype_src_DIR}/include )

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------

file(GLOB SHADOW_SRC
	"../../../engine/source/*.cpp"
    "../../../engine/source/entity/*.cpp"
    "../../../engine/source/entity/components/*.cpp"
    "../../../engine/source/entity/systems/*.cpp"
    "../../../engine/source/font/*.cpp"
    "../../../engine/source/gui/*.cpp"
    "../../../engine/source/io/FileSystem.cpp"
    "../../../engine/source/io/android/AndroidFileSystem.cpp"
    "../../../engine/source/platform/android/*.cpp"
    "../../../engine/source/resources/*.cpp"
    "../../../engine/source/scene/*.cpp"
    "../../../engine/source/scene/ModelLoader/*.cpp"
    "../../../engine/source/video/*.cpp"
    "../../../engine/source/video/Managers/*.cpp"
    "../../../engine/source/video/GLContext/*.cpp"
    "../../../engine/source/video/GLES20/*.cpp"
    "../../../engine/source/video/GLES20/Managers/*.cpp"
    "../../../engine/source/video/GLES20/Batching/*.cpp"
    "../../../engine/source/video/TextureLoader/*.cpp"
    "../../../libs/pugixml/src/*.cpp"
)

if (USE_VULKAN_API)
    file(GLOB VULKAN_SRC
        "../../../engine/source/video/Vulkan/*.cpp"
        )
    list(APPEND SHADOW_SRC ${VULKAN_SRC})
endif(USE_VULKAN_API)

add_library( # Sets the name of the library.
             shadow-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.

             ${SHADOW_SRC}
             src/main/cpp/ShadowJNI.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.



find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       shadow-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       android
                       GLESv2
                       EGL
                       lib-freetype)

include_directories(../../../engine/source)
include_directories(../../../libs/egl/include)
include_directories(../../../libs/pugixml/src)
include_directories(../../../libs/stb/include)
include_directories(../../../libs/tinyobjloader/include)
include_directories(../../../libs/vulkan/include)
#include_directories(../../../libs/freetype/include)
