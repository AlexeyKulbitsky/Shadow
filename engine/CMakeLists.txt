cmake_minimum_required(VERSION 3.4.1)

project (ShadowEngine)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0015 NEW)

set(PROJECT_NAME Shadow)

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

set(SH_FONT_SRC
  source/font/Font.cpp
  source/font/FontManager.cpp
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
    source/io/win32/WindowsFileSystem.cpp
  )
elseif(ANDROID)
  set(SH_PLATFORM_SPECIFIC_IO_SRC
    source/io/android/AndroidFileSystem.cpp
  )
elseif(APPLE)
  set(SH_PLATFORM_SPECIFIC_IO_SRC
    source/io/mac/MacFileSystem.mm
  )
endif()

set(SH_IO_SRC
  source/io/File.cpp
  source/io/FileSystem.cpp
  ${SH_PLATFORM_SPECIFIC_IO_SRC}
)

set(SH_JOBS_SRC
  source/jobs/JobQueue.cpp
  source/jobs/JobScheduler.cpp
  source/jobs/Test.cpp
  source/jobs/Worker.cpp
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
    source/platform/win32/Win32Device.cpp
  )
elseif(ANDROID)
  set(SH_PLATFORM_SRC
    source/platform/android/AndroidDevice.cpp
  )
elseif(APPLE)
  set(SH_PLATFORM_SRC
    source/platform/mac/MacDevice.mm
  )
endif()

set(SH_RESOURCES_SRC
  source/resources/ResourceManager.cpp
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

set(SH_SERIALIZATION_SRC
  source/serialization/ObjectFactory.cpp
  source/serialization/Property.cpp
  source/serialization/Serializable.cpp
  source/serialization/Serializer.cpp
  source/serialization/XMLSerializer.cpp
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
    source/video/GL/GLContext/WGLContextManager.cpp
  )
elseif(ANDROID)
  set(SH_OPENGL_CONTEXT_SRC
    source/video/GL/GLContext/EGLContextManager.cpp
  )
elseif(APPLE)
  set(SH_OPENGL_CONTEXT_SRC
    source/video/GL/GLContext/NSGLContextManager.mm
  )
endif()

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

set(PROJECT_SOURCE_FILES
  ${SH_ENTITY_SRC}
  ${SH_FONT_SRC}
  ${SH_GUI_SRC}
  ${SH_IO_SRC}
  ${SH_JOBS_SRC}
  ${SH_MATH_SRC}
  ${SH_PLATFORM_SRC}
  ${SH_RESOURCES_SRC}
  ${SH_SCENE_SRC}
  ${SH_SERIALIZATION_SRC}
  ${SH_VIDEO_SRC}
  ${SH_OPENGL_COMMON_SRC}
  ${SH_OPENGL_CONTEXT_SRC}
  ${SH_GLES20_SRC}
  ${SH_COMMON_SRC}
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
include_directories(../libs/tinyobjloader)

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
elseif(ANDROID)
  add_subdirectory(../libs/minizip "${CMAKE_CURRENT_BINARY_DIR}/ThirdParty/minizip")
  include_directories(../libs/minizip)
  target_link_libraries(${PROJECT_NAME} minizip)
elseif(APPLE)
  find_library(COCOA_LIB Cocoa)
  target_link_libraries(${PROJECT_NAME} ${COCOA_LIB})
endif()
