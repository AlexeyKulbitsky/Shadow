cmake_minimum_required(VERSION 3.4.1)

project (ShadowEngine)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0015 NEW)

set(PROJECT_NAME Shadow)

set(SH_ENTITY_INCLUDE
  source/entity/Component.h
  source/entity/ComponentsFactory.h
  source/entity/Entity.h
  source/entity/System.h
  source/entity/components/CameraComponent.h
  source/entity/components/LightComponent.h
  source/entity/components/RenderComponent.h
  source/entity/components/ScriptComponent.h
  source/entity/components/TerrainComponent.h
  source/entity/components/TransformComponent.h
  source/entity/systems/LightSystem.h
  source/entity/systems/RenderSystem.h
  source/entity/systems/ScriptSystem.h
  source/entity/systems/TerrainSystem.h
  source/entity/systems/TransformSystem.h
)

set(SH_ENTITY_SRC
  source/entity/Component.cpp
  source/entity/ComponentsFactory.cpp
  source/entity/Entity.cpp
  source/entity/System.cpp
  source/entity/components/CameraComponent.cpp
  source/entity/components/LightComponent.cpp
  source/entity/components/RenderComponent.cpp
  source/entity/components/ScriptComponent.cpp
  source/entity/components/TerrainComponent.cpp
  source/entity/components/TransformComponent.cpp
  source/entity/systems/LightSystem.cpp
  source/entity/systems/RenderSystem.cpp
  source/entity/systems/ScriptSystem.cpp
  source/entity/systems/TerrainSystem.cpp
  source/entity/systems/TransformSystem.cpp
)

set(SH_FONT_INCLUDE
  source/font/Font.h
  source/font/FontManager.h
)

set(SH_FONT_SRC
  source/font/Font.cpp
  source/font/FontManager.cpp
)

set(SH_GUI_INCLUDE
  source/gui/Button.h
  source/gui/ButtonGroup.h
  source/gui/ComboBox.h
  source/gui/FloatLineEdit.h
  source/gui/GuiManager.h
  source/gui/HorizontalLayout.h
  source/gui/ImageWidget.h
  source/gui/IntLineEdit.h
  source/gui/Label.h
  source/gui/Layout.h
  source/gui/LineEdit.h
  source/gui/Menu.h
  source/gui/MenuBar.h
  source/gui/ScrollWidget.h
  source/gui/SliderWidget.h
  source/gui/Sprite.h
  source/gui/SpriteManager.h
  source/gui/SpriteWidget.h
  source/gui/Style.h
  source/gui/Text.h
  source/gui/ToolBar.h
  source/gui/TreeWidget.h
  source/gui/VerticalLayout.h
  source/gui/Widget.h
  source/gui/Window.h
)

set(SH_GUI_SRC
  source/gui/Button.cpp
  source/gui/ButtonGroup.cpp
  source/gui/ComboBox.cpp
  source/gui/FloatLineEdit.cpp
  source/gui/GuiManager.cpp
  source/gui/HorizontalLayout.cpp
  source/gui/ImageWidget.cpp
  source/gui/IntLineEdit.cpp
  source/gui/Label.cpp
  source/gui/Layout.cpp
  source/gui/LineEdit.cpp
  source/gui/Menu.cpp
  source/gui/MenuBar.cpp
  source/gui/ScrollWidget.cpp
  source/gui/SliderWidget.cpp
  source/gui/Sprite.cpp
  source/gui/SpriteManager.cpp
  source/gui/SpriteWidget.cpp
  source/gui/Style.cpp
  source/gui/Text.cpp
  source/gui/ToolBar.cpp
  source/gui/TreeWidget.cpp
  source/gui/VerticalLayout.cpp
  source/gui/Widget.cpp
  source/gui/Window.cpp
)

if(WIN32)
  set(SH_PLATFORM_SPECIFIC_IO_SRC
    source/io/win32/WindowsFileSystem.h
    source/io/win32/WindowsFileSystem.cpp
  )
elseif(ANDROID)
  set(SH_PLATFORM_SPECIFIC_IO_SRC
    source/io/android/AndroidFileSystem.h
    source/io/android/AndroidFileSystem.cpp
  )
elseif(APPLE)
  set(SH_PLATFORM_SPECIFIC_IO_SRC
    source/io/mac/MacFileSystem.h
    source/io/mac/MacFileSystem.mm
  )
endif()

set(SH_IO_SRC
  source/io/File.h
  source/io/File.cpp
  source/io/FileSystem.h
  source/io/FileSystem.cpp
  ${SH_PLATFORM_SPECIFIC_IO_SRC}
)

set(SH_JOBS_INCLUDE
  source/jobs/JobQueue.h
  source/jobs/JobScheduler.h
  source/jobs/Test.h
  source/jobs/Worker.h
)

set(SH_JOBS_SRC
  source/jobs/JobQueue.cpp
  source/jobs/JobScheduler.cpp
  source/jobs/Test.cpp
  source/jobs/Worker.cpp
)

set(SH_MATH_INCLUDE
  source/math/AABB.h
  source/math/Matrix3.h
  source/math/Matrix4.h
  source/math/Plane.h
  source/math/Quaternion.h
  source/math/Rect.h
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

set(SH_RESOURCES_INCLUDE
  source/resources/ResourceManager.h
)

set(SH_RESOURCES_SRC
  source/resources/ResourceManager.cpp
)

set(SH_SCENE_INCLUDE
  source/scene/Camera.h
  source/scene/GeometryGenerator.h
  source/scene/Light.h
  source/scene/Mesh.h
  source/scene/MeshBase.h
  source/scene/Model.h
  source/scene/ModelBase.h
  source/scene/Picker.h
  source/scene/Scene.h
  source/scene/SceneManager.h
  source/scene/ModelLoader/AssimpModelLoader.h
  source/scene/ModelLoader/ModelLoader.h
  source/scene/ModelLoader/TinyObjModelLoader.h
)

set(SH_SCENE_SRC
  source/scene/Camera.cpp
  source/scene/GeometryGenerator.cpp
  source/scene/Light.cpp
  source/scene/Mesh.cpp
  source/scene/MeshBase.cpp
  source/scene/Model.cpp
  source/scene/ModelBase.cpp
  source/scene/Picker.cpp
  source/scene/Scene.cpp
  source/scene/SceneManager.cpp
  source/scene/ModelLoader/AssimpModelLoader.cpp
  source/scene/ModelLoader/ModelLoader.cpp
  source/scene/ModelLoader/TinyObjModelLoader.cpp
)

set(SH_SERIALIZATION_INCLUDE
  source/serialization/ObjectFactory.h
  source/serialization/Property.h
  source/serialization/Serializable.h
  source/serialization/Serializer.h
  source/serialization/XMLSerializer.h
)

set(SH_SERIALIZATION_SRC
  source/serialization/ObjectFactory.cpp
  source/serialization/Property.cpp
  source/serialization/Serializable.cpp
  source/serialization/Serializer.cpp
  source/serialization/XMLSerializer.cpp
)

set(SH_VIDEO_INCLUDE
  source/video/BlendingState.h
  source/video/CommandBuffer.h
  source/video/CommandPool.h
  source/video/DepthStencilState.h
  source/video/Driver.h
  source/video/GpuParams.h
  source/video/GpuPipelineParamsInfo.h
  source/video/Image.h
  source/video/IndexBuffer.h
  source/video/Material.h
  source/video/MaterialParam.h
  source/video/MaterialParams.h
  source/video/Painter.h
  source/video/RasterizationState.h
  source/video/Renderable.h
  source/video/RenderBatchManager.h
  source/video/RenderPass.h
  source/video/RenderPipeline.h
  source/video/RenderTarget.h
  source/video/RenderTechnique.h
  source/video/Sampler.h
  source/video/Shader.h
  source/video/Texture.h
  source/video/TextureManager.h
  source/video/VertexBuffer.h
  source/video/VertexDeclaration.h
  source/video/TextureLoader/TextureLoader.h  
  source/video/Managers/CommandBufferManager.h
  source/video/Managers/HardwareBufferManager.h
  source/video/Managers/RenderStateManager.h
)

set(SH_VIDEO_SRC
  source/video/BlendingState.cpp
  source/video/CommandBuffer.cpp
  source/video/CommandPool.cpp
  source/video/DepthStencilState.cpp
  source/video/Driver.cpp
  source/video/GpuParams.cpp
  source/video/GpuPipelineParamsInfo.cpp
  source/video/Image.cpp
  source/video/IndexBuffer.cpp
  source/video/Material.cpp
  source/video/MaterialParam.cpp
  source/video/MaterialParams.cpp
  source/video/Painter.cpp
  source/video/RasterizationState.cpp
  source/video/Renderable.cpp
  source/video/RenderBatchManager.cpp
  source/video/RenderPass.cpp
  source/video/RenderPipeline.cpp
  source/video/RenderTarget.cpp
  source/video/RenderTechnique.cpp
  source/video/Sampler.cpp
  source/video/Shader.cpp
  source/video/Texture.cpp
  source/video/TextureManager.cpp
  source/video/VertexBuffer.cpp
  source/video/VertexDeclaration.cpp
  source/video/TextureLoader/TextureLoader.cpp

  source/video/Managers/CommandBufferManager.cpp
  source/video/Managers/HardwareBufferManager.cpp
  source/video/Managers/RenderStateManager.cpp
)

set(SH_OPENGL_COMMON_INCLUDE
  source/video/GL/GLCommandBuffer.h
  source/video/GL/GLIndexBuffer.h
  source/video/GL/GLRenderTarget.h
  source/video/GL/GLShader.h
  source/video/GL/GLTexture.h
  source/video/GL/GLVertexBuffer.h
  source/video/GL/Managers/GLCommandBufferManager.h
  source/video/GL/Managers/GLHardwareBufferManager.h
  source/video/GL/Managers/GLRenderStateManager.h
  source/video/GL/Managers/GLTextureManager.h
)

set(SH_OPENGL_COMMON_SRC
  source/video/GL/GLCommandBuffer.cpp
  source/video/GL/GLIndexBuffer.cpp
  source/video/GL/GLRenderTarget.cpp
  source/video/GL/GLShader.cpp
  source/video/GL/GLTexture.cpp
  source/video/GL/GLVertexBuffer.cpp
  source/video/GL/Managers/GLCommandBufferManager.cpp
  source/video/GL/Managers/GLHardwareBufferManager.cpp
  source/video/GL/Managers/GLRenderStateManager.cpp
  source/video/GL/Managers/GLTextureManager.cpp
)

if(WIN32)
  set(SH_OPENGL_CONTEXT_SRC
    source/video/GL/GLContext/WGLContextManager.h
    source/video/GL/GLContext/WGLContextManager.cpp
  )
elseif(ANDROID)
  set(SH_OPENGL_CONTEXT_SRC
    source/video/GL/GLContext/EGLContextManager.h
    source/video/GL/GLContext/EGLContextManager.cpp
  )
elseif(APPLE)
  set(SH_OPENGL_CONTEXT_SRC
    source/video/GL/GLContext/NSGLContextManager.h
    source/video/GL/GLContext/NSGLContextManager.mm
  )
endif()

set(SH_GLES20_INCLUDE
  source/video/GL/GLES20/Batching/GLES20MaterialSorter.h
  source/video/GL/GLES20/Batching/GLES20RenderBatch.h
  source/video/GL/GLES20/Batching/GLES20RenderBatchManager.h
  source/video/GL/GLES20/Batching/GLES20StateDifference.h
  source/video/GL/GLES20/Managers/GLES20RenderStateManager.h
  source/video/GL/GLES20/GLES20Driver.h
  source/video/GL/GLES20/GLES20RenderPipeline.h
  source/video/GL/GLES20/GLES20VertexDEclaration.h
)

set(SH_GLES20_SRC
  source/video/GL/GLES20/Batching/GLES20MaterialSorter.cpp
  source/video/GL/GLES20/Batching/GLES20RenderBatch.cpp
  source/video/GL/GLES20/Batching/GLES20RenderBatchManager.cpp
  source/video/GL/GLES20/Batching/GLES20StateDifference.cpp
  source/video/GL/GLES20/Managers/GLES20RenderStateManager.cpp
  source/video/GL/GLES20/GLES20Driver.cpp
  source/video/GL/GLES20/GLES20RenderPipeline.cpp
  source/video/GL/GLES20/GLES20VertexDEclaration.cpp
)

set(SH_COMMON_INCLUDE
  source/Application.h
  source/Device.h
  source/DynamicLibrary.h
  source/InputManager.h
  source/pempek_assert.h
  source/Shadow.h
  source/System.h
  source/Variant.h
)

set(SH_COMMON_SRC
  source/Application.cpp
  source/Device.cpp
  source/DynamicLibrary.cpp
  source/InputManager.cpp
  source/pempek_assert.cpp
  source/Shadow.cpp
  source/System.cpp
  source/Variant.cpp
)

set(SH_NEW_GUI_INCLUDE
  source/newGui/Button.h  
)

set(SH_NEW_GUI_SRC
  source/newGui/Button.cpp 
)

set(PROJECT_SOURCE_FILES
  ${SH_ENTITY_INCLUDE}
  ${SH_ENTITY_SRC}
  ${SH_FONT_INCLUDE}
  ${SH_FONT_SRC}
  ${SH_GUI_INCLUDE}
  ${SH_GUI_SRC}
  ${SH_IO_SRC}
  ${SH_JOBS_INCLUDE}
  ${SH_JOBS_SRC}
  ${SH_MATH_INCLUDE}
  ${SH_MATH_SRC}
  ${SH_PLATFORM_SRC}
  ${SH_RESOURCES_INCLUDE}
  ${SH_RESOURCES_SRC}
  ${SH_SCENE_INCLUDE}
  ${SH_SCENE_SRC}
  ${SH_SERIALIZATION_INCLUDE}
  ${SH_SERIALIZATION_SRC}
  ${SH_VIDEO_INCLUDE}
  ${SH_VIDEO_SRC}
  ${SH_OPENGL_COMMON_INCLUDE}
  ${SH_OPENGL_COMMON_SRC}
  ${SH_OPENGL_CONTEXT_SRC}
  ${SH_GLES20_INCLUDE}
  ${SH_GLES20_SRC}
  ${SH_COMMON_INCLUDE}
  ${SH_COMMON_SRC}

  ${SH_NEW_GUI_INCLUDE}
  ${SH_NEW_GUI_SRC}
)

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
#add_library(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
#target_compile_definitions(${PROJECT_NAME} PUBLIC SHADOW_STATIC_LIB)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCE_FILES})
target_compile_definitions(${PROJECT_NAME} PRIVATE SHADOW_EXPORTS)

# Add DirectX
if (WIN32)
  include_directories($ENV{DXSDK_DIR}/Include)
endif(WIN32)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

# Add pugixml
add_subdirectory(../libs/pugixml "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/pugixml")
include_directories( ../libs/pugixml/src )
set_property(TARGET pugixml PROPERTY FOLDER CommonLibs)
target_link_libraries(${PROJECT_NAME} pugixml)

# Add freetype
add_subdirectory(../libs/freetype "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/freetype")
include_directories(../libs/freetype/include)
set_property(TARGET freetype PROPERTY FOLDER CommonLibs)
target_link_libraries(${PROJECT_NAME} freetype)

# Add Vulkan
if (USE_VULKAN_API)
  include_directories(../libs/vulkan/include)

  set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "Builds glslangValidator and spirv-remap" FORCE)
  set(ENABLE_HLSL OFF CACHE BOOL "Enables HLSL input support" FORCE)

  add_subdirectory(../libs/glslang "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/glslang")
  include_directories(../libs/glslang)
  include_directories(../libs/glslang/StandAlone)

  set_property(TARGET glslang PROPERTY FOLDER CommonLibs)
  set_property(TARGET SPIRV PROPERTY FOLDER CommonLibs)
  set_property(TARGET SPVRemapper PROPERTY FOLDER CommonLibs)
  set_property(TARGET OGLCompiler PROPERTY FOLDER CommonLibs)
  set_property(TARGET OSDependent PROPERTY FOLDER CommonLibs)

  target_link_libraries(${PROJECT_NAME} glslang SPIRV SPVRemapper)
endif()

# Add stb
include_directories(../libs/stb)

# Add Tiny OBJ loader
add_subdirectory(../libs/tinyobjloader "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/tinyobjloader")
include_directories(../libs/tinyobjloader)
set_property(TARGET tinyobjloader PROPERTY FOLDER CommonLibs)
target_link_libraries(${PROJECT_NAME} tinyobjloader)

# Add EGL
include_directories(../libs/egl/include)

# Add GLEW
set(BUILD_UTILS OFF CACHE BOOL "utilities" FORCE)
add_subdirectory(../libs/glew/build/cmake "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/glew")
include_directories(../libs/glew/include)
target_link_libraries(${PROJECT_NAME} glew_s)
set_property(TARGET glew_s PROPERTY FOLDER CommonLibs)
set_property(TARGET glew PROPERTY FOLDER CommonLibs)

# Add assimp
if (USE_ASSIMP_API)
  add_definitions(-DSHADOW_ASSIMP_LOADER)

  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built in addition to the library." FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library." FORCE)
  add_subdirectory(../libs/assimp "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/assimp")
  include_directories(../libs/assimp/include)
  include_directories("${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/assimp/include")
  target_link_libraries(${PROJECT_NAME} assimp)

  set_property(TARGET assimp PROPERTY FOLDER CommonLibs)
  set_property(TARGET IrrXML PROPERTY FOLDER CommonLibs)
  #set_property(TARGET zlibstatic PROPERTY FOLDER CommonLibs)
  #set_property(TARGET UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTY FOLDER CommonLibs)
  set_property(TARGET uninstall PROPERTY FOLDER CommonLibs)
endif()

set_property(TARGET dist PROPERTY FOLDER CommonLibs)

if(WIN32)
  add_definitions(-DSHADOW_WINDOWS)
elseif(ANDROID)
  add_subdirectory(../libs/minizip "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/minizip")
  include_directories(../libs/minizip)
  target_link_libraries(${PROJECT_NAME} minizip)
elseif(APPLE)
  find_library(COCOA_LIB Cocoa)
  target_link_libraries(${PROJECT_NAME} ${COCOA_LIB})
endif()
