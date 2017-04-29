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
		void SetVisible(bool visible) { m_visible = visible; }
		bool IsVisible() const { return m_visible; }

		void SetEnabled(bool enabled) { m_enabled = enabled; }
		bool IsEnabled() const { return m_enabled; }

		virtual void Load(const pugi::xml_node& node) { }
		virtual void GetGeometry(GuiBatchData& data) { }
		virtual void GetText(GuiBatchData& data) { }
		virtual void SetPosition(u32 x, u32 y) { }
		virtual void SetSize(const math::Vector2u& size) { }
		virtual void SetWidth(u32 width) {}
		virtual void SetHeight(u32 height) {}
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) { return false; }

	protected:
		bool m_visible = true;
		bool m_enabled = true;
		std::vector<float> m_batchData;
	};

} // gui

} // sh

#endif