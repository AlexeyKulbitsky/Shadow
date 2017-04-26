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

	class GuiElement
	{
	public:
		virtual void Load(const pugi::xml_node& node) { }

		const std::vector<float>& GetBatchData() { return m_batchData; }
		virtual void SetPosition(u32 x, u32 y) = 0;
		virtual void SetWidth(u32 width) = 0;
		virtual void SetHeight(u32 height) = 0;

		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) = 0;

	protected:
		std::vector<float> m_batchData;
	};

} // gui

} // sh

#endif