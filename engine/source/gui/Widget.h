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

		// Maximum size
		void SetMaximumWidth(u32 width) { m_maxSize.x = width; }
		void SetMaximumHeight(u32 height) { m_maxSize.y = height; }
		void SetMaximumSize(const math::Vector2u& size) { m_maxSize = size; }
		u32 GetMaximumWidth() const { return m_maxSize.x; }
		u32 GetMaximumHeight() const { return m_maxSize.y; }
		const math::Vector2u& GetMaximumSize() const { return m_maxSize; }

		// Minimum size
		void SetMinimumWidth(u32 width) { m_minSize.x = width; }
		void SetMinimumHeight(u32 height) { m_minSize.y = height; }
		void SetMinimumSize(const math::Vector2u& size) { m_minSize = size; }
		u32 GetMinimumWidth() const { return m_minSize.x; }
		u32 GetMinimumHeight() const { return m_minSize.y; }
		const math::Vector2u& GetMinimumSize() const { return m_minSize; }

		const math::Recti& GetRect() const { return m_rect; }

		void SetLayout(const LayoutPtr& layout);
		const LayoutPtr& GetLayout() const { return m_layout; }

		virtual void Load(const pugi::xml_node& node);
		virtual void Render(video::Painter* painter);
		virtual void SetPosition(s32 x, s32 y);
		virtual void SetSize(const math::Vector2i& size);
		virtual void SetWidth(s32 width);
		virtual void SetHeight(s32 height);
		virtual bool ProcessEvent(GUIEvent& ev);

		void SetFocus(bool focus) { m_isInFocus = focus; }
		bool IsInFocus() const { return m_isInFocus; }

		// Method is called when widget contents geometry must be recalculated
		virtual void UpdateLayout();

	protected:
		LayoutPtr m_layout;

		math::Vector2u m_maxSize = math::Vector2u(std::numeric_limits<s32>::max());
		math::Vector2u m_minSize = math::Vector2u(0U);

		math::Recti m_rect;
		bool m_visible = true;
		bool m_enabled = true;
		bool m_isInFocus = false;
	};

} // gui

} // sh

#endif