#ifndef SHADOW_GUI_ELEMENT_INCLUDE
#define SHADOW_GUI_ELEMENT_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{
	struct GeometryData
	{
		u32 offset = 0U;
	};

	
	struct GuiBatchData;

	class GuiElement
	{
	public:
		GuiElement();
		virtual ~GuiElement() {}

		void SetVisible(bool visible) { m_visible = visible; }
		bool IsVisible() const { return m_visible; }

		void SetEnabled(bool enabled) { m_enabled = enabled; }
		bool IsEnabled() const { return m_enabled; }

		const math::Rectu& GetRect() const { return m_rect; }

		virtual void Load(const pugi::xml_node& node) { }
		virtual void GetGeometry(GuiBatchData& data);
		virtual void GetTextGeometry(GuiBatchData& data) { }
		virtual void SetPosition(u32 x, u32 y);
		virtual void SetSize(const math::Vector2u& size);
		virtual void SetWidth(u32 width);
		virtual void SetHeight(u32 height);
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) { return false; }
		virtual bool ProcessKeyboardInput(KeyboardEventType type, KeyCode code) { return false; }

	protected:
		virtual void UpdatePosition();
		virtual void UpdateUV(const math::Vector2f& leftUp, const math::Vector2f& rightDown);
		virtual void UpdateColor(const math::Vector3f& color);

	protected:
		math::Rectu m_rect;
		bool m_visible = true;
		bool m_enabled = true;
		std::vector<float> m_batchData;
	};

} // gui

} // sh

#endif