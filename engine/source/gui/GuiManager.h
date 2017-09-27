#ifndef SHADOW_GUI_MANAGER_INCLUDE
#define SHADOW_GUI_MANAGER_INCLUDE

#include "../Globals.h"
#include "../video/GpuParams.h"

namespace sh
{

namespace gui
{
	class Widget;

	struct GuiBatchData
	{
		std::vector<float> vertices;
		u32 verticesCount = 0U;
		std::vector<u32> indices;
	};

	struct GuiBatch
	{
		video::VertexBufferPtr vertexBuffer;
		video::IndexBufferPtr indexBuffer;
		video::VertexInputDeclarationPtr inputDeclaration;
		video::MaterialPtr material;
		video::CommandBufferPtr commandBuffer;
		video::GpuParamMatrix4f orthoMatrix;
	};

	struct GUIEvent;

	class GuiManager : public Singleton <GuiManager>
	{
		friend class Button;
	public:
		GuiManager();
		~GuiManager();

		void Init();
		void Update(u32 delta);
		void Render();

		void LoadGui(const char* filename);
		void AddChild(const SPtr<Widget>& child);
		void AddChildFront(const SPtr<Widget>& child);

		void SetFont(const FontPtr& font);
		const FontPtr& GetFont() const { return m_font; }

		size_t GetChildrenCount() const { return m_children.size(); }
		void RemoveChild(size_t index);
		void RemoveAllChildren() { m_children.clear(); }
		
		void UpdateMatrices();

		void SetStyle(const StylePtr& style);
		const StylePtr& GetStyle() const { return m_style; }

		void SetFocusWidget(const SPtr<Widget>& widget);
		const SPtr<Widget>& GetFocusWidget() const { return m_focusWidget; }

		const video::MaterialPtr& GetDefaultMaterial() const { return m_defaultMaterial; }
		const video::MaterialPtr& GetTextMaterial() const { return m_textMaterial; }

		// Events
		bool ProcessEvent(GUIEvent& ev);

	private:
		FontPtr m_font;

		List<SPtr<Widget>> m_children;

		StylePtr m_style;

		SPtr<Widget> m_focusWidget;
		Queue<SPtr<Widget>> m_focusWidgetsQueue;
		///////////////////////////////////

		video::MaterialPtr m_defaultMaterial;
		video::MaterialPtr m_textMaterial;
	};

} // gui

} // sh

#endif