#ifndef SHADOW_FORWARD_INCLUDE
#define SHADOW_FORWARD_INCLUDE

#include <memory>


#define DEFINE_CLASS_PTR(C)					\
class C;									\
typedef std::shared_ptr<C> C##Ptr;			\
typedef std::unique_ptr<C> C##UPtr;		


namespace sh
{
	namespace video
	{
		DEFINE_CLASS_PTR(BlendingState)
		DEFINE_CLASS_PTR(DepthStencilState)
		DEFINE_CLASS_PTR(HardwareBuffer)
		DEFINE_CLASS_PTR(IndexBuffer)
		DEFINE_CLASS_PTR(Material)
		DEFINE_CLASS_PTR(Painter)
		DEFINE_CLASS_PTR(RasterizationState)
		DEFINE_CLASS_PTR(RenderBatch)
		DEFINE_CLASS_PTR(RenderBatchManager)
		DEFINE_CLASS_PTR(RenderCommand)
		DEFINE_CLASS_PTR(RenderPipeline)
		DEFINE_CLASS_PTR(RenderTarget)
		DEFINE_CLASS_PTR(RenderTechnique)
		DEFINE_CLASS_PTR(Sampler)
		DEFINE_CLASS_PTR(ShaderProgram)
		DEFINE_CLASS_PTR(Texture)
		DEFINE_CLASS_PTR(Uniform)
		DEFINE_CLASS_PTR(UniformsBatch)
		DEFINE_CLASS_PTR(UniformBuffer)
		DEFINE_CLASS_PTR(VertexBuffer)
		DEFINE_CLASS_PTR(VertexDeclaration)
		DEFINE_CLASS_PTR(VertexInputDeclaration)
	}

	namespace scene
	{
		DEFINE_CLASS_PTR(Camera)
		DEFINE_CLASS_PTR(GeometryGenerator)
		DEFINE_CLASS_PTR(Light)
		DEFINE_CLASS_PTR(Mesh)
		DEFINE_CLASS_PTR(MeshBase)
		DEFINE_CLASS_PTR(Model)
		DEFINE_CLASS_PTR(ModelBase)
		DEFINE_CLASS_PTR(ModelLoader)
		DEFINE_CLASS_PTR(Picker)
		DEFINE_CLASS_PTR(SceneManager)
		DEFINE_CLASS_PTR(TinyObjModelLoader)
	}

	namespace io
	{
		DEFINE_CLASS_PTR(FileSystem)
	}

	DEFINE_CLASS_PTR(Component)
	DEFINE_CLASS_PTR(LightComponent)
	DEFINE_CLASS_PTR(RenderComponent)
	DEFINE_CLASS_PTR(TransformComponent)
	DEFINE_CLASS_PTR(ComponentFactory)
	DEFINE_CLASS_PTR(Entity)
	DEFINE_CLASS_PTR(System)
	DEFINE_CLASS_PTR(LightSystem)
	DEFINE_CLASS_PTR(RenderSystem)
	DEFINE_CLASS_PTR(TransformSystem)

	DEFINE_CLASS_PTR(InputManager)
	DEFINE_CLASS_PTR(Resource)
	DEFINE_CLASS_PTR(ResourceManager)
}

#endif