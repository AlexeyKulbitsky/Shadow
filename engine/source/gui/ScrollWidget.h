#ifndef SHADOW_SCROLL_WIDGET_INCLUDE
#define SHADOW_SCROLL_WIDGET_INCLUDE

#include "Widget.h"
#include "Button.h"

namespace sh
{

namespace gui
{
	
	class SHADOW_API ScrollWidget : public Widget
	{
	public:
		class ScrollBar : public Button
		{
			friend class ScrollWidget;
		public:
			ScrollBar();
			ScrollBar(const SpritePtr& defaultSprite,
				const SpritePtr& pressedSprite,
				const SpritePtr& hoveredSprite,
				const SpritePtr& backgroundSprite);
			virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
			virtual void Render(video::Painter* painter) override;
			virtual void RenderBackground(video::Painter* painter) override;
			virtual void RenderText(video::Painter* painter) override;

			bool IsDragStarted() const;

		protected:
			ScrollWidget* m_scrollWidget = nullptr;
			bool m_dragStarted = false;
			sh::math::Vector2Int m_startPos;
			SpritePtr m_backgroundSprite;
		};

		ScrollWidget(); 
		ScrollWidget(const SPtr<ScrollBar>& scrollBar);
		virtual ~ScrollWidget();

		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetSize(const math::Vector2Int& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;
		virtual void UpdateLayout() override;

	protected:
		void UpdateScrollBars();
		void RenderScrollBars(video::Painter* painter);

	protected:
		math::Rect m_fullRect;
		s32 m_scrollSpeed = 15;

		sh::SPtr<ScrollBar> m_verticalScrollBar;
		sh::SPtr<ScrollBar> m_horizontalScrollBar;

		friend class ScrollBar;
		friend class Style;
	};

} // gui

} // sh

#endif