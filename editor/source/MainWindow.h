#ifndef SHADOW_EDITOR_MAIN_WINDOW
#define SHADOW_EDITOR_MAIN_WINDOW

#if 0
#include <QMainWindow>
#include <Shadow.h>

class GraphicsWidget;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void NewFile();
	void OpenFile();
	void SaveFile();
	void SaveAsFile();
	void Update();
	void SetSelectedEntity(sh::Entity* entity);

protected:
	virtual void keyPressEvent(QKeyEvent * e);
	virtual void keyReleaseEvent(QKeyEvent * e);
	virtual void closeEvent(QCloseEvent *e);

private:
	void CreateActions();
	void CreateMenu();
	void CreateAssetsWidget();
	void CreateInspectorWidget();
	void CreateToolBar();
	void InitDevice();
	void ClearLayout(QLayout *layout);

private:
	QAction* newAction;
	QAction* openAction;
	QAction* saveAction;
	QAction* saveAsAction;
	QAction* exitAction;

	QAction* copyAction;
	QAction* deleteAction;
	QAction* pasteAction;
	QAction* cutAction;

	QMenu* fileMenu;
	QMenu* editMenu;
	QMenu* toolsMenu;
	QMenu* optionsMenu;
	QMenu* contextMenu;

	QDockWidget *assetsWidget;

	QDockWidget *inspectorWidget;
	QVBoxLayout *inspectorLayout;

	QToolBar* m_mainToolBar;
	GraphicsWidget* m_graphicsWidget;
	QTimer *m_timer;

	sh::scene::SceneManager* m_sceneManager = nullptr;
};
#endif

#include "gizmo/Gizmo.h"

#include <Shadow.h>

class MainWindow
{
public:
	MainWindow();

	void Update();

	void OpenScene();
	void SaveScene();
	void Close();

	void OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code);
	void OnKeyboardEvent(sh::KeyboardEventType type, sh::KeyCode code);

private:
	sh::gui::MenuBarPtr m_menuBar;
	sh::gui::ToolBarPtr m_toolBar;

	sh::SPtr<Gizmo> m_gizmo;
};

#endif
