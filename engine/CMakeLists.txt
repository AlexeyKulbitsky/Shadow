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

# Math
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
  source/math/math.h
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

# Platform
if(WIN32)
  set(SH_PLATFORM_SRC
    source/platform/win32/Win32Device.h
    source/platform/win32/Win32Device.cpp
    source/platform/win32/WinProc.cpp
  )
elseif(ANDROID)
  set(SH_PLATFORM_SRC
    source/platform/android/AndroidDevice.h
    source/platform/android/AndroidDevice.cpp
  )
elseif(APPLE)
  if(IOS)
    set(SH_PLATFORM_SRC
      source/platform/ios/iOSDevice.h
      source/platform/ios/iOSDevice.mm
    )
  elseif(MACOSX)
    set(SH_PLATFORM_SRC
      source/platform/mac/MacDevice.h
      source/platform/mac/MacDevice.mm
    )
  endif()
  
endif()

# Device
set(SH_DEVICE_INCLUDE
  source/device/Application.h
  source/device/Device.h 
  source/device/CreationParameters.h
)

set(SH_DEVICE_SOURCE
  source/device/Application.cpp
  source/device/Device.cpp
)

# Common
set(SH_COMMON_INCLUDE
  source/Shadow.h
  source/prerequisities/PlatformDefines.h
  source/prerequisities/Prerequisities.h
  source/prerequisities/Forward.h
)

set(SH_COMMON_SRC
  source/Shadow.cpp
)

# Video
set(SH_VIDEO_INCLUDE
  source/video/RenderSystem.h
  source/video/Shader.h
  source/video/Common.h
  source/video/VertexBuffer.h
  source/video/IndexBuffer.h
  source/video/RenderPipeline.h
  source/video/Managers/HardwareBufferManager.h
  source/video/Managers/RenderStateManager.h
  source/video/VertexDeclaration.h
  source/video/VertexInputDeclaration.h
  source/video/DepthStencilState.h
  source/video/BlendingState.h
  source/video/RasterizationState.h
  source/video/GpuParamsDescription.h
  source/video/GpuPipelineParamsInfo.h
  source/video/GpuParams.h
  source/video/Sampler.h
)

set(SH_VIDEO_SRC
  source/video/RenderSystem.cpp
  source/video/Shader.cpp
  source/video/VertexBuffer.cpp
  source/video/IndexBuffer.cpp
  source/video/RenderPipeline.cpp
  source/video/Managers/HardwareBufferManager.cpp
  source/video/Managers/RenderStateManager.cpp
  source/video/VertexDeclaration.cpp
  source/video/VertexInputDeclaration.cpp
  source/video/DepthStencilState.cpp
  source/video/BlendingState.cpp
  source/video/RasterizationState.cpp
  source/video/GpuPipelineParamsInfo.cpp
  source/video/GpuParams.cpp
  source/video/Sampler.cpp
)

# Render
set(SH_RENDER_INCLUDE
  source/render/Material.h
  source/render/MaterialParam.h
  source/render/MaterialParams.h
)

set(SH_RENDER_SRC
  source/render/Material.cpp
  source/render/MaterialParam.cpp
  source/render/MaterialParams.cpp
)

# Resources
set(SH_RESOURCES_INCLUDE
  source/resources/Resource.h
  source/resources/ResourceManager.h
)

set(SH_RESOURCES_SRC
  source/Resources/ResourceManager.cpp
)

# Utility
set(SH_UTILITY_INCLUDE
  source/utility/Singleton.h
  source/utility/Event.h
)

# OpenGL
set(SH_GL_API_INCLUDE
  source/video/GL/GLDebug.h
  source/video/GL/GLCommon.h
  source/video/GL/GLRenderSystemCommon.h
  source/video/GL/GLShader.h
  source/video/GL/GLVertexBuffer.h 
  source/video/GL/GLIndexBuffer.h
  source/video/GL/Managers/GLHardwareBufferManager.h
  source/video/GL/Managers/GLRenderStateManager.h
)

set(SH_GL_API_SRC
  source/video/GL/GLDebug.cpp
  source/video/GL/GLRenderSystemCommon.cpp
  source/video/GL/GLShader.cpp
  source/video/GL/GLVertexBuffer.cpp 
  source/video/GL/GLIndexBuffer.cpp
  source/video/GL/Managers/GLHardwareBufferManager.cpp
  source/video/GL/Managers/GLRenderStateManager.cpp
)

set(SH_GL_CONTEXT_INCLUDE
  source/video/GL/GLContext/GLContextManager.h
)

if(WIN32)
  set(SH_GL_CONTEXT_PLATFORM_INCLUDE
    source/video/GL/GLContext/WGLContextManager.h
  )
  set(SH_GL_CONTEXT_PLATFORM_SRC
    source/video/GL/GLContext/WGLContextManager.cpp
  )
elseif(ANDROID)
  set(SH_GL_CONTEXT_PLATFORM_INCLUDE
    source/video/GL/GLContext/EGLContextManager.h
  )
  set(SH_GL_CONTEXT_PLATFORM_SRC
    source/video/GL/GLContext/EGLContextManager.cpp
  )
endif()

set(SH_GLES20_INCLUDE
  source/video/GL/GLES20/GLES20RenderSystem.h
  source/video/GL/GLES20/GLES20RenderPipeline.h
  source/video/GL/GLES20/Managers/GLES20RenderStateManager.h
  source/video/GL/GLES20/GLES20VertexInputDeclaration.h
)

set(SH_GLES20_SRC
  source/video/GL/GLES20/GLES20RenderSystem.cpp
  source/video/GL/GLES20/GLES20RenderPipeline.cpp
  source/video/GL/GLES20/Managers/GLES20RenderStateManager.cpp
  source/video/GL/GLES20/GLES20VertexInputDeclaration.cpp
)

set(SH_OPEN_GL_SOURCES
  ${SH_GL_API_INCLUDE}
  ${SH_GL_API_SRC}
  ${SH_GL_CONTEXT_INCLUDE}
  ${SH_GL_CONTEXT_PLATFORM_INCLUDE}
  ${SH_GL_CONTEXT_PLATFORM_SRC}

  # GLES20
  ${SH_GLES20_INCLUDE}
  ${SH_GLES20_SRC}
)

set(PROJECT_SOURCE_FILES
  ${SH_MATH_INCLUDE}
  ${SH_MATH_SRC}
  ${SH_PLATFORM_SRC}
  ${SH_DEVICE_INCLUDE}
  ${SH_DEVICE_SOURCE}
  ${SH_COMMON_INCLUDE}
  ${SH_COMMON_SRC}
  ${SH_VIDEO_INCLUDE}
  ${SH_VIDEO_SRC}
  ${SH_RENDER_INCLUDE}
  ${SH_RENDER_SRC}
  ${SH_RESOURCES_INCLUDE}
  ${SH_RESOURCES_SRC}
  ${SH_UTILITY_INCLUDE}

  # OpenGL
  ${SH_OPEN_GL_SOURCES}
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
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE SHADOW_EXPORTS)
  target_compile_definitions(${PROJECT_NAME} PRIVATE SHADOW_STATIC_LIB)
endif()

if(WIN32)
  add_definitions(-DSHADOW_WINDOWS)

  # Standart Windows Opengl lib
  find_library(OPENGL_LIB opengl32)
  target_link_libraries(${PROJECT_NAME} ${OPENGL_LIB})

  # Add GLEW
  set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
  add_subdirectory(../libs/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/glew")
  include_directories(../libs/glew/include)
  target_link_libraries(${PROJECT_NAME} glew_s)
  set_property(TARGET glew_s PROPERTY FOLDER CommonLibs)
  set_property(TARGET glew PROPERTY FOLDER CommonLibs)

elseif(ANDROID)
  #add_subdirectory(../libs/minizip "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/minizip")
  #include_directories(../libs/minizip)
  #target_link_libraries(${PROJECT_NAME} minizip)
  target_link_libraries(${PROJECT_NAME} android GLESv2 EGL)

elseif(APPLE)
  if(IOS)
    set_property (TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS "-ObjC")
    set_xcode_property(${PROJECT_NAME} GCC_GENERATE_DEBUGGING_SYMBOLS YES "All")
    find_library(UIKIT_LIB UIKit)
    target_link_libraries(${PROJECT_NAME} ${UIKIT_LIB})
    find_library(METAL_LIB Metal)
    target_link_libraries(${PROJECT_NAME} ${METAL_LIB})
    find_library(QUARTZCORE_LIB QuartzCore)
    target_link_libraries(${PROJECT_NAME} ${QUARTZCORE_LIB})
  elseif(MACOSX)
    find_library(COCOA_LIB Cocoa)
    target_link_libraries(${PROJECT_NAME} ${COCOA_LIB})
  endif()
  
endif()
