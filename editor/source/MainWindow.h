#ifndef SHADOW_EDITOR_MAIN_WINDOW
#define SHADOW_EDITOR_MAIN_WINDOW

#include "gizmo/Gizmo.h"
#include "gizmo/MoveGizmo.h"
#include "gui/TransformWidget.h"

#include <Shadow.h>

class MainWindow : public sh::Application
{
public:
	MainWindow();

	void Update();

	void OpenScene();
	void SaveScene();
	void Close();

	void OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code);
	void OnMouseWeelEvent(int delta);
	void OnKeyboardEvent(sh::KeyboardEventType type, sh::KeyCode code);
	void OnWindowResized(int width, int height);

	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(sh::u64 delta) override;

private:
	sh::gui::MenuBarPtr m_menuBar;
	sh::gui::ToolBarPtr m_toolBar;

	sh::SPtr<TransformWidget> m_transformWidget;

	// Gizmos
	sh::SPtr<Gizmo> m_gizmo;
	sh::SPtr<Gizmo> m_defaultGizmo;
	sh::SPtr<MoveGizmo> m_moveGizmo;

	sh::Entity* m_cameraTargetEntity = nullptr;
};

#endif
