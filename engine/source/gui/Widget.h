#ifndef SHADOW_WIDGET_INCLUDE
#define SHADOW_WIDGET_INCLUDE

#include "../Globals.h"
#include "GuiBatch.h"

namespace pugi
{
	class xml_node;
}

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
		PointerDoubleClick,
		Wheel,
		KeyDown,
		KeyUp
	};

	struct SHADOW_API GUIEvent
	{
		EventType type;
		s32 x = 0;
		s32 y = 0;
		s32 deltaX = 0;
		s32 deltaY = 0;
		s32 delta = 0;
		s32 keyCode = 0;
		MouseCode mouseButtonCode = MouseCode::ButtonLeft;
	};

	class SHADOW_API Widget : public std::enable_shared_from_this<Widget>
	{
		friend class GuiManager;
	public:
		Widget();
		virtual ~Widget();

		void SetVisible(bool visible) { m_visible = visible; }
		bool IsVisible() const { return m_visible; }
		void SetEnabled(bool enabled) { m_enabled = enabled; }
		bool IsEnabled() const { return m_enabled; }

		// Maximum size
		void SetMaximumWidth(s32 width) { m_maxSize.x = width; }
		void SetMaximumHeight(s32 height) { m_maxSize.y = height; }
		void SetMaximumSize(const math::Vector2Int& size) { m_maxSize = size; }
		s32 GetMaximumWidth() const { return m_maxSize.x; }
		s32 GetMaximumHeight() const { return m_maxSize.y; }
		const math::Vector2Int& GetMaximumSize() const { return m_maxSize; }

		// Minimum size
		void SetMinimumWidth(s32 width) { m_minSize.x = width; }
		void SetMinimumHeight(s32 height) { m_minSize.y = height; }
		void SetMinimumSize(const math::Vector2Int& size) { m_minSize = size; }
		s32 GetMinimumWidth() const { return m_minSize.x; }
		s32 GetMinimumHeight() const { return m_minSize.y; }
		const math::Vector2Int& GetMinimumSize() const { return m_minSize; }

		const math::Rect& GetRect() const { return m_rect; }

		// Layout
		void SetLayout(const LayoutPtr& layout);
		const LayoutPtr& GetLayout() const { return m_layout; }

		// Parent
		void SetParent(Widget* parent) { m_parent = parent; }
		Widget* GetParent() const { return m_parent; }
		virtual void Load(const pugi::xml_node& node);
		virtual void Render(video::Painter* painter);
		virtual void RenderBackground(video::Painter* painter);
		virtual void RenderText(video::Painter* painter);
		virtual void CollectBatches(GuiLayerBatch& batch);
		virtual void SetRect(const math::Rect& rect);
		virtual void SetPosition(s32 x, s32 y);
		const sh::math::Vector2Int& GetPosition() const { return m_rect.upperLeftCorner; }
		virtual void SetSize(const math::Vector2Int& size);
		virtual void SetWidth(s32 width);
		virtual void SetHeight(s32 height);
		virtual bool ProcessEvent(GUIEvent& ev);

		//void SetFocus(bool focus);
		bool IsInFocus() const;

		void SetMaterial(const video::MaterialPtr& material) { m_material = material; }
		const video::MaterialPtr& GetMaterial() const { return m_material; }

		// Method is called when widget contents geometry must be recalculated
		virtual void UpdateLayout();

	protected:
		LayoutPtr m_layout;
		Widget* m_parent = nullptr;
		math::Vector2Int m_maxSize = math::Vector2Int(std::numeric_limits<int32_t>::max());
		math::Vector2Int m_minSize = math::Vector2Int(0);

		video::MaterialPtr m_material;

		math::Rect m_rect;
		bool m_visible = true;
		bool m_enabled = true;
	};

} // gui

} // sh

#endif