#ifndef SHADOW_GUI_MANAGER_INCLUDE
#define SHADOW_GUI_MANAGER_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{
	class GuiElement;

	struct GuiBatch
	{
		video::VertexBufferPtr vertexBuffer;
		video::IndexBufferPtr indexBuffer;
		video::VertexInputDeclarationPtr inputDeclaration;
		video::MaterialPtr material;
		video::CommandBufferPtr commandBuffer;
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

		void LoadGui(const char* filename);
		void AddChild(const SPtr<GuiElement>& child);

		const SPtr<GuiElement>& GetChild(size_t i) const { return m_children[i]; }
		size_t GetChildrenCount() const { return m_children.size(); }

		void OnMouseEvent(int x, int y, MouseEventType type, MouseCode code);
		void OnKeyboardEvent(KeyboardEventType type, KeyCode code);

	private:
		GuiBatch m_mainBatch;
		GuiBatch m_textBatch;

		std::vector<SPtr<GuiElement>> m_children;
	};

} // gui

} // sh

#endif