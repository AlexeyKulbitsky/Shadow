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

		void AddChild(const SPtr<GuiElement>& child);

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