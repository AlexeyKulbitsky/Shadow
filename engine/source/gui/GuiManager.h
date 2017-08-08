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

	class GuiManager : public Singleton <GuiManager>
	{
		friend class Button;
	public:
		GuiManager();
		~GuiManager();

		void Init();
		void Update(u32 delta);
		void Render();

		void CreateMenuBar();
		void CreateToolBar();

		const MenuBarPtr& GetMenuBar() const { return m_menuBar; }
		const ToolBarPtr& GetToolBar() const { return m_toolBar; }

		void LoadGui(const char* filename);
		void AddChild(const SPtr<Widget>& child);

		void SetFont(const FontPtr& font);
		const FontPtr& GetFont() const { return m_font; }

		const SPtr<Widget>& GetChild(size_t i) const { return m_children[i]; }
		size_t GetChildrenCount() const { return m_children.size(); }
		void RemoveAllChildren() { m_children.clear(); }
		
		void UpdateMatrices();

		void SetStyle(const StylePtr& style);
		const StylePtr& GetStyle() const { return m_style; }

		// Events
		bool ProcessInput(u32 x, u32 y, MouseEventType type);
		bool ProcessKeyboardInput(KeyboardEventType type, KeyCode code);

	private:
		void InitMainBatch();
		void InitTextBatch();

	private:
		GuiBatchData m_mainBatchData;
		GuiBatch m_mainBatch;

		GuiBatchData m_textBatchData;
		GuiBatch m_textBatch;

		FontPtr m_font;

		MenuBarPtr m_menuBar;
		ToolBarPtr m_toolBar;
		std::vector<SPtr<Widget>> m_children;

		StylePtr m_style;
	};

} // gui

} // sh

#endif