#ifndef SHADOW_EDITOR_MAIN_WINDOW
#define SHADOW_EDITOR_MAIN_WINDOW

#include <QMainWindow>

class GraphicsWidget;

namespace sh
{
	namespace scene
	{
		class SceneManager;
	}
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void NewFile();
	void OpenFile();
	void Update();

private:
	void CreateActions();
	void CreateMenu();
	void CreateAssetsWidget();
	void CreateInspectorWidget();
	void InitDevice();

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

	GraphicsWidget* m_graphicsWidget;
	QTimer *m_timer;

	sh::scene::SceneManager* m_sceneManager = nullptr;
};

#endif
