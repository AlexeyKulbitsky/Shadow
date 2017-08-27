#ifndef SHADOW_WIDGET_INCLUDE
#define SHADOW_WIDGET_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{
	struct GuiBatchData;

	enum class EventType : u32
	{
		PointerDown,
		PointerUp,
		PointerMove,
		Wheel,
		KeyDown,
		KeyUp
	};

	struct GUIEvent
	{
		EventType type;
		s32 x = 0;
		s32 y = 0;
		s32 deltaX = 0;
		s32 deltaY = 0;
		s32 delta = 0;
		s32 keyCode = 0;
	};

	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:
		Widget();
		virtual ~Widget();

		void SetVisible(bool visible) { m_visible = visible; }
		bool IsVisible() const { return m_visible; }
		void SetEnabled(bool enabled) { m_enabled = enabled; }
		bool IsEnabled() const { return m_enabled; }
		void SetMaximumWidth(u32 width) { m_maximumWidth = width; }
		u32 GetMaximumWidth() const { return m_maximumWidth; }
		void SetMaximumHeight(u32 height) { m_maximumHeight = height; }
		u32 GetMaximumHeight() const { return m_maximumHeight; }

		const math::Recti& GetRect() const { return m_rect; }

		void SetLayout(const LayoutPtr& layout);
		const LayoutPtr& GetLayout() const { return m_layout; }

		virtual void Load(const pugi::xml_node& node);
		virtual void Render(video::Painter* painter);
		virtual void SetPosition(s32 x, s32 y);
		virtual void SetSize(const math::Vector2u& size);
		virtual void SetWidth(u32 width);
		virtual void SetHeight(u32 height);
		virtual bool ProcessEvent(GUIEvent& ev);

		virtual void SetMargins(u32 top, u32 right, u32 bottom, u32 left);

		void SetFocus(bool focus) { m_isInFocus = focus; }
		bool IsInFocus() const { return m_isInFocus; }

		// Method is called when widget contents geometry must be recalculated
		virtual void UpdateLayout();

	protected:
		virtual void UpdatePosition();
		virtual void UpdateUV(const math::Vector2f& leftUp, const math::Vector2f& rightDown);
		virtual void UpdateColor(const math::Vector4f& color);

	protected:
		LayoutPtr m_layout;
		u32 m_topMargin = 0U;
		u32 m_rightMargin = 0U;
		u32 m_bottomMargin = 0U;
		u32 m_leftMargin = 0U;

		u32 m_maximumHeight = 0U;
		u32 m_maximumWidth = 0U;

		math::Recti m_rect;
		bool m_visible = true;
		bool m_enabled = true;
		bool m_isInFocus = false;
		std::vector<float> m_batchData;
	};

} // gui

} // sh

#endif