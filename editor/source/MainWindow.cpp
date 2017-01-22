#include "MainWindow.h"
#include <QApplication>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QSplitter>
#include <QTimer>
#include <qdockwidget.h>
#include <qfilesystemmodel.h>
#include <qtreeview.h>
#include <QScrollArea>
#include <qtoolbar.h>
#include <QActionGroup>

#include "gui\GraphicsWidget.h"
#include "gui\ExpandableWidget.h"
#include "gui\decorators\EditorComponentsFactory.h"
#include "gui\decorators\ComponentDecorator.h"
#include "gui\decorators\TransformComponent\TransformComponentDecorator.h"
#include "gui\decorators\RenderComponent\RenderComponentDecorator.h"

#include <Shadow.h>
#include <scene\Camera.h>
#include <scene\SceneManager.h>
#include <scene\ModelLoader\TinyObjModelLoader.h>
#include <entity\Entity.h>



MainWindow::MainWindow(QWidget *parent)
{
	resize(QSize(640, 480));
	CreateActions();
	CreateMenu();
	

	QSplitter *m_splitter = new QSplitter(Qt::Vertical, this);
	m_splitter->setObjectName("mainsplitter");
	
	m_graphicsWidget = new GraphicsWidget(this);	
	m_graphicsWidget->resize(QSize(640, 480));
	m_splitter->addWidget(m_graphicsWidget);
	setCentralWidget(m_splitter);

	connect(m_graphicsWidget, SIGNAL(EntitySelected(sh::Entity*)), this, SLOT(SetSelectedEntity(sh::Entity*)));
	CreateToolBar();
	InitDevice();

	m_graphicsWidget->Init();

	//CreateAssetsWidget();
	CreateInspectorWidget();

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(Update()), Qt::QueuedConnection);
	m_timer->start(10);
}

//////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{

}

//////////////////////////////////////////////////////////////////////////

void MainWindow::NewFile()
{

}

//////////////////////////////////////////////////////////////////////////

void MainWindow::OpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("0pen Spreadsheet"), QApplication::applicationDirPath(),
		tr("Spreadsheet files (*.xml)"));

	if (!fileName.isEmpty())
	{
		sh::Device::GetInstance()->GetSceneManager()->LoadScene(fileName.toStdString().c_str());
	}
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::Update()
{
	m_graphicsWidget->Render();
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::SetSelectedEntity(sh::Entity* entity)
{
	if (entity)
	{
		sh::Component* component = entity->GetComponent(sh::Component::Type::TRANSFORM);
		if (component)
		{
			TransformComponentDecorator* decorator = dynamic_cast<TransformComponentDecorator*>(component);
			ExpandableWidget* widget = decorator->GetParametersWidget();
			inspectorLayout->addWidget(widget);
		}
		component = entity->GetComponent(sh::Component::Type::RENDER);
		if (component)
		{
			RenderComponentDecorator* decorator = dynamic_cast<RenderComponentDecorator*>(component);
			ExpandableWidget* widget = decorator->GetParametersWidget();
			inspectorLayout->addWidget(widget);
		}
	}
	else
	{
		ClearLayout(inspectorLayout);
	}
	
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::CreateActions()
{
	newAction = new QAction(tr("&New"), this);	
	newAction->setShortcut(QKeySequence::New);
	newAction->setStatusTip(tr("Create new project"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(NewFile()));

	openAction = new QAction(tr("&Open..."), this);
	openAction->setShortcut(QKeySequence::Open);
	openAction->setStatusTip(tr("Open project"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(OpenFile()));

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save project"));
	//connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	saveAsAction = new QAction(tr("&Save as..."), this);
	saveAsAction->setShortcut(QKeySequence::SaveAs);
	saveAsAction->setStatusTip(tr("Save project as..."));
	//connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

	exitAction = new QAction(tr("Quit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Close application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::CreateMenu()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addSeparator();
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::CreateAssetsWidget()
{
	assetsWidget = new QDockWidget(tr("File explorer"), this);
	assetsWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

	QFileSystemModel *model = new QFileSystemModel;

	sh::String workingDir = sh::Device::GetInstance()->GetFileSystem()->GetWorkingDirectory();
    QString path = QDir::currentPath();
	model->setRootPath(path);

    QTreeView *tree = new QTreeView(assetsWidget);
    tree->setModel(model);
	tree->setRootIndex(model->index(path));

	assetsWidget->setWidget(tree);
	addDockWidget(Qt::LeftDockWidgetArea, assetsWidget);
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::CreateInspectorWidget()
{
	inspectorWidget = new QDockWidget(tr("Inspector"), this);
	inspectorWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

	QScrollArea* inspectorScrollArea = new QScrollArea(inspectorWidget);
	inspectorScrollArea->setFocusPolicy(Qt::StrongFocus);
	inspectorWidget->setWidget(inspectorScrollArea);
	
	QWidget* inspector = new QWidget(inspectorWidget);
	inspectorLayout = new QVBoxLayout(inspector);
	inspectorLayout->setAlignment(Qt::AlignTop);
	inspector->setLayout(inspectorLayout);
	inspectorScrollArea->setWidget(inspector);

	inspectorScrollArea->setWidgetResizable(true);
	addDockWidget(Qt::RightDockWidgetArea, inspectorWidget);
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::CreateToolBar()
{
	m_mainToolBar = new QToolBar("Main toolbar");
	addToolBar(m_mainToolBar);
	QString workingDir = QApplication::applicationDirPath();

	QAction* selectAction = new QAction("Select", this);	
	selectAction->setIcon(QIcon(workingDir + "/../../../../../data/icons/select.png"));
	selectAction->setCheckable(true);

	QAction* moveAction = new QAction("Move", this);			
	moveAction->setIcon(QIcon(workingDir + "/../../../../../data/icons/move.png"));
	moveAction->setCheckable(true);

	QAction* rotateAction = new QAction("Rotate", this);			
	rotateAction->setIcon(QIcon(workingDir + "/../../../../../data/icons/rotate.png"));
	rotateAction->setCheckable(true);

	QAction* scaleAction = new QAction("Scale", this);			
	scaleAction->setIcon(QIcon(workingDir + "/../../../../../data/icons/scale.png"));
	scaleAction->setCheckable(true);
	scaleAction->setEnabled(false);

	QActionGroup* transformGroup = new QActionGroup(this);	
	transformGroup->addAction(selectAction);
	transformGroup->addAction(moveAction);
	transformGroup->addAction(rotateAction);
	transformGroup->addAction(scaleAction);

	connect(transformGroup, SIGNAL(triggered(QAction*)), m_graphicsWidget, SLOT(TransformActionChanged(QAction*)));

	m_mainToolBar->addAction(selectAction);
	m_mainToolBar->addAction(moveAction);
	m_mainToolBar->addAction(rotateAction);
	m_mainToolBar->addAction(scaleAction);
	
	selectAction->setChecked(true);

	m_mainToolBar->addSeparator();
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::InitDevice()
{
	WId id = m_graphicsWidget->winId();
	HWND hwnd = (HWND)id;

	sh::CreationParameters params;
	params.width = width();
	params.height = height();
	params.WinId = (void*)hwnd;
	params.driverType = sh::video::DriverType::OPENGL_ES_2_0;
	sh::Device* device = sh::CreateDevice(params);

	sh::scene::ModelLoader* modelLoader = new sh::scene::TinyObjModelLoader();
	sh::scene::ModelLoader::SetInstance(modelLoader);

	sh::scene::SceneManager* sceneMgr = new sh::scene::SceneManager();
	sceneMgr->SetComponentsFactory(new EditorComponentsFactory());
	device->SetSceneManager(sceneMgr);

	m_sceneManager = sceneMgr;

	sh::io::FileSystem* fs = device->GetFileSystem();
	fs->AddFolder(sh::String("../../../../../data"));
	fs->Init();

	sh::io::FileInfo finfo = fs->FindFile(sh::String("test_scene.xml"));
	if (finfo.name == "")
		return;

	sceneMgr->LoadScene(finfo.absolutePath.c_str());

	sh::scene::Camera* camera = new sh::scene::Camera();
	camera->SetProjection(3.1415926535f / 4.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	camera->SetPosition(sh::math::Vector3f(0.0f));
	sceneMgr->SetCamera(camera);
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::ClearLayout(QLayout *layout)
{
	while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);
        if (item->layout() != NULL)
        {
            ClearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget() != NULL)
        {
            delete item->widget();
        }
    }
}

//////////////////////////////////////////////////////////////////////////
