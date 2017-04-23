#ifndef SHADOW_GUI_MANAGER_INCLUDE
#define SHADOW_GUI_MANAGER_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{
	class GuiElement;

	class GuiManager : public Singleton <GuiManager>
	{
		friend class Button;
	public:
		GuiManager();
		~GuiManager();

		void Init();
		void Update(u32 delta);
		void Render();

		void OnMouseEvent(int x, int y, MouseEventType type, MouseCode code);
		void OnKeyboardEvent(KeyboardEventType type, KeyCode code);

	private:
		video::VertexBufferPtr m_vertexBuffer;
		video::IndexBufferPtr m_indexBuffer;
		video::VertexInputDeclarationPtr m_inputDeclaration;
		video::MaterialPtr m_material;

		std::vector<SPtr<GuiElement>> m_children;
	};

} // gui

} // sh

#endif