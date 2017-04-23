#ifndef SHADOW_INCLUDE
#define SHADOW_INCLUDE

#include "CompileConfig.h"
#include "Device.h"
#include "Event.h"

#include "gui/GuiManager.h"
#include "gui/Button.h"

#include "entity/Component.h"
#include "entity/ComponentsFactory.h"
#include "entity/Entity.h"
#include "entity/System.h"
#include "entity/components/LightComponent.h"
#include "entity/components/RenderComponent.h"
#include "entity/components/TransformComponent.h"
#include "entity/systems/LightSystem.h"
#include "entity/systems/RenderSystem.h"
#include "entity/systems/TransformSystem.h"

#include "math/smath.h"

#include "resources/Resource.h"
#include "resources/ResourceManager.h"

#include "scene/Camera.h"
#include "scene/GeometryGenerator.h"
#include "scene/Light.h"
#include "scene/Mesh.h"
#include "scene/MeshBase.h"
#include "scene/Model.h"
#include "scene/ModelBase.h"
#include "scene/Picker.h"
#include "scene/SceneManager.h"
#include "scene/ModelLoader/ModelLoader.h"
#include "scene/ModelLoader/TinyObjModelLoader.h"

#include "video/GLContext/GLContextManager.h"
#include "video/Driver.h"
#include "video/BlendingState.h"
#include "video/CommandPool.h"
#include "video/DepthStencilState.h"
#include "video/IndexBuffer.h"
#include "video/Material.h"
#include "video/Painter.h"
#include "video/RasterizationState.h"
#include "video/Renderable.h"
#include "video/RenderPipeline.h"
#include "video/RenderTarget.h"
#include "video/RenderTechnique.h"
#include "video/Sampler.h"
#include "video/Texture.h"
#include "video/VertexBuffer.h"
#include "video/VertexDeclaration.h"

namespace sh
{
	// Create usual device
	/*SHADOW_API*/ Device* /*SH_CALLCONV*/ CreateDevice
	(
		video::DriverType driverType = video::DriverType::OPENGL_ES_2_0,
		u32 width = 800, u32 height = 600
	);

	// Create device with more extended configuration
	/*SHADOW_API*/ Device* /*SH_CALLCONV*/ CreateDevice(const CreationParameters& parameters);
}

#endif
