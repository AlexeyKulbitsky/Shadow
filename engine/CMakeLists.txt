cmake_minimum_required(VERSION 3.4.1)

project (ShadowEngine)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0015 NEW)

if(WIN32)
  SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
elseif(APPLE)
  SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
endif()

set(PROJECT_NAME Shadow)

set(SH_MATH_INCLUDE
  source/math/AABB.h
  source/math/Constants.h
  source/math/FunctionsCommon.h
  source/math/FunctionsRayIntersect.h
  source/math/Matrix3.h
  source/math/Matrix4.h
  source/math/Plane.h
  source/math/Quaternion.h
  source/math/Rect.h
  source/math/smath.h
  source/math/Sphere.h
  source/math/Vector2.h
  source/math/Vector2Int.h
  source/math/Vector3.h
  source/math/Vector3Int.h
  source/math/Vector4.h
  source/math/Vector4Int.h
)

set(SH_MATH_SRC
  source/math/AABB.cpp
  source/math/Matrix3.cpp
  source/math/Matrix4.cpp
  source/math/Plane.cpp
  source/math/Quaternion.cpp
  source/math/Rect.cpp
  source/math/Sphere.cpp
  source/math/Vector2.cpp
  source/math/Vector2Int.cpp
  source/math/Vector3.cpp
  source/math/Vector3Int.cpp
  source/math/Vector4.cpp
  source/math/Vector4Int.cpp
)

if(WIN32)
  set(SH_PLATFORM_SRC
    source/platform/win32/Win32Device.h
    source/platform/win32/Win32Device.cpp
  )
elseif(ANDROID)
  set(SH_PLATFORM_SRC
    source/platform/android/AndroidDevice.h
    source/platform/android/AndroidDevice.cpp
  )
elseif(APPLE)
  set(SH_PLATFORM_SRC
    source/platform/mac/MacDevice.h
    source/platform/mac/MacDevice.mm
  )
endif()


set(SH_COMMON_INCLUDE
  source/Application.h
  source/Device.h
  source/Shadow.h

  source/CommonTypes.h
  source/CreationParameters.h

  source/common/PlatformDefines.h
  source/common/Prerequisities.h
  source/Globals.h
)

set(SH_COMMON_SRC
  source/Application.cpp
  source/Device.cpp
  source/Shadow.cpp
)

set(PROJECT_SOURCE_FILES
  ${SH_MATH_INCLUDE}
  ${SH_MATH_SRC}
  ${SH_PLATFORM_SRC}
  ${SH_COMMON_INCLUDE}
  ${SH_COMMON_SRC}
)

include_directories(source)

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

# Add Shadow engine library
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCE_FILES})
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE SHADOW_EXPORTS)
endif()

if(WIN32)
  add_definitions(-DSHADOW_WINDOWS)
elseif(ANDROID)
  #add_subdirectory(../libs/minizip "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/minizip")
  #include_directories(../libs/minizip)
  #target_link_libraries(${PROJECT_NAME} minizip)

  target_link_libraries(${PROJECT_NAME} android)
elseif(APPLE)
  find_library(COCOA_LIB Cocoa)
  target_link_libraries(${PROJECT_NAME} ${COCOA_LIB})
endif()
