# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.


file(GLOB SHADOW_SRC
	"../../../engine/source/*.cpp"
    "../../../engine/source/entity/*.cpp"
    "../../../engine/source/entity/components/*.cpp"
    "../../../engine/source/entity/systems/*.cpp"
    "../../../engine/source/io/*.cpp"
    "../../../engine/source/platform/android/*.cpp"
    "../../../engine/source/resources/*.cpp"
    "../../../engine/source/scene/*.cpp"
    "../../../engine/source/scene/ModelLoader/*.cpp"
    "../../../engine/source/video/*.cpp"
    "../../../engine/source/video/GLContext/*.cpp"
    "../../../engine/source/video/GLES20/*.cpp"
    "../../../engine/source/video/TextureLoader/*.cpp"
    "../../../libs/pugixml/src/*.cpp"
)

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
                       EGL)

include_directories(../../../engine/source)
include_directories(../../../libs/egl/include)
include_directories(../../../libs/pugixml/src)
include_directories(../../../libs/stb/include)
include_directories(../../../libs/tinyobjloader/include)
include_directories(../../../libs/vulkan/include)
