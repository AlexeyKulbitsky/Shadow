#ifndef SHADOW_FORWARD_INCLUDE
#define SHADOW_FORWARD_INCLUDE

#include <memory>


#define DEFINE_CLASS_PTR(C)					\
class C;									\
typedef std::shared_ptr<C> C##Ptr;			\
typedef std::unique_ptr<C> C##UPtr;			\
typedef std::weak_ptr<C> C##WPtr;


namespace sh
{
	namespace video
	{
		DEFINE_CLASS_PTR(BlendingState)
		DEFINE_CLASS_PTR(CommandBuffer)
		DEFINE_CLASS_PTR(DepthStencilState)
		DEFINE_CLASS_PTR(GpuParams)
		DEFINE_CLASS_PTR(GpuPipelineParamsInfo)
		DEFINE_CLASS_PTR(HardwareBuffer)
		DEFINE_CLASS_PTR(IndexBuffer)
		DEFINE_CLASS_PTR(Material)
		DEFINE_CLASS_PTR(MaterialParams)
		DEFINE_CLASS_PTR(Painter)
		DEFINE_CLASS_PTR(RasterizationState)
		DEFINE_CLASS_PTR(RenderBatch)
		DEFINE_CLASS_PTR(RenderBatchManager)
		DEFINE_CLASS_PTR(Renderable)
		DEFINE_CLASS_PTR(RenderPipeline)
		DEFINE_CLASS_PTR(RenderTarget)
		DEFINE_CLASS_PTR(RenderTechnique)
		DEFINE_CLASS_PTR(Sampler)
		DEFINE_CLASS_PTR(Shader)
		DEFINE_CLASS_PTR(ShaderProgram)
		DEFINE_CLASS_PTR(Texture)
		DEFINE_CLASS_PTR(Uniform)
		DEFINE_CLASS_PTR(UniformsBatch)
		DEFINE_CLASS_PTR(UniformBuffer)
		DEFINE_CLASS_PTR(VertexBuffer)
		DEFINE_CLASS_PTR(VertexDeclaration)
		DEFINE_CLASS_PTR(VertexInputDeclaration)

		struct GpuParamsDescription;
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

	namespace gui
	{
		DEFINE_CLASS_PTR(GuiElement)
		DEFINE_CLASS_PTR(Button)
		DEFINE_CLASS_PTR(HorizontalLayout)
		DEFINE_CLASS_PTR(VerticalLayout)
		DEFINE_CLASS_PTR(Sprite)
		DEFINE_CLASS_PTR(Label)
		DEFINE_CLASS_PTR(Layout)
		DEFINE_CLASS_PTR(LayoutItem)
		DEFINE_CLASS_PTR(LineEdit)
		DEFINE_CLASS_PTR(FloatLineEdit)
		DEFINE_CLASS_PTR(MenuBar)
		DEFINE_CLASS_PTR(Menu)
		DEFINE_CLASS_PTR(ScrollWidget)
		DEFINE_CLASS_PTR(Style)
		DEFINE_CLASS_PTR(Text)
		DEFINE_CLASS_PTR(ToolBar)
		DEFINE_CLASS_PTR(Widget)
		DEFINE_CLASS_PTR(Window)
		DEFINE_CLASS_PTR(ComboBox)
		DEFINE_CLASS_PTR(SliderWidget)
		DEFINE_CLASS_PTR(TreeWidget)
		DEFINE_CLASS_PTR(TreeItem)
		DEFINE_CLASS_PTR(ImageWidget)
		DEFINE_CLASS_PTR(SpriteWidget)
	}

	DEFINE_CLASS_PTR(Component)
	DEFINE_CLASS_PTR(LightComponent)
	DEFINE_CLASS_PTR(RenderComponent)
	DEFINE_CLASS_PTR(TransformComponent)
	DEFINE_CLASS_PTR(TerrainComponent)
	DEFINE_CLASS_PTR(ComponentFactory)
	DEFINE_CLASS_PTR(Entity)
	DEFINE_CLASS_PTR(System)
	DEFINE_CLASS_PTR(LightSystem)
	DEFINE_CLASS_PTR(RenderSystem)
	DEFINE_CLASS_PTR(TransformSystem)

	DEFINE_CLASS_PTR(InputManager)
	DEFINE_CLASS_PTR(Resource)
	DEFINE_CLASS_PTR(ResourceManager)

	DEFINE_CLASS_PTR(Font)
}

#endif