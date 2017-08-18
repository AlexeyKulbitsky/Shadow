#ifndef SHADOW_WIDGET_INCLUDE
#define SHADOW_WIDGET_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{
	struct GuiBatchData;

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

		const math::Rectu& GetRect() const { return m_rect; }

		void SetLayout(const LayoutPtr& layout);
		const LayoutPtr& GetLayout() const { return m_layout; }

		virtual void Load(const pugi::xml_node& node);
		virtual void GetGeometry(GuiBatchData& data);
		virtual void GetTextGeometry(GuiBatchData& data);
		virtual void Render(video::Painter* painter);
		virtual void SetPosition(u32 x, u32 y);
		virtual void SetSize(const math::Vector2u& size);
		virtual void SetWidth(u32 width);
		virtual void SetHeight(u32 height);
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type);
		virtual bool ProcessKeyboardInput(KeyboardEventType type, KeyCode code);

		virtual void SetMargins(u32 top, u32 right, u32 bottom, u32 left);

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

		math::Rectu m_rect;
		bool m_visible = true;
		bool m_enabled = true;
		std::vector<float> m_batchData;
	};

} // gui

} // sh

#endif