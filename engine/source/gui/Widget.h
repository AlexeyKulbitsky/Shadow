#ifndef SHADOW_WIDGET_INCLUDE
#define SHADOW_WIDGET_INCLUDE

#include "GuiElement.h"

namespace sh
{

namespace gui
{

	class Widget : public GuiElement
	{
	public:
		Widget();
		virtual ~Widget();

		void SetLayout(const LayoutPtr& layout);
		const LayoutPtr& GetLayout() const { return m_layout; }

		virtual void Load(const pugi::xml_node& node) override;
		virtual void GetGeometry(GuiBatchData& data) override;
		virtual void GetTextGeometry(GuiBatchData& data) override;
		virtual void SetPosition(u32 x, u32 y) override;
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;
		virtual bool ProcessKeyboardInput(KeyboardEventType type, KeyCode code) override;

		void SetMaximumWidth(u32 width) { m_maximumWidth = width; }
		u32 GetMaximumWidth() const { return m_maximumWidth; }
		void SetMaximumHeight(u32 height) { m_maximumHeight = height; }
		u32 GetMaximumHeight() const { return m_maximumHeight; }

		virtual void SetMargins(u32 top, u32 right, u32 bottom, u32 left);

	//protected:
		virtual void UpdateLayout();

	protected:
		LayoutPtr m_layout;
		u32 m_topMargin = 0U;
		u32 m_rightMargin = 0U;
		u32 m_bottomMargin = 0U;
		u32 m_leftMargin = 0U;

		u32 m_maximumHeight = 0U;
		u32 m_maximumWidth = 0U;
	};

} // gui

} // sh

#endif