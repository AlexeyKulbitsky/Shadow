#include "MainWindow.h"
#include <QApplication>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QSplitter>
#include <QTimer>

#include "gui\GraphicsWidget.h"

#include <Shadow.h>
#include <scene\Camera.h>
#include <scene\SceneManager.h>
#include <scene\ModelLoader\TinyObjModelLoader.h>

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

	InitDevice();

	m_graphicsWidget->Init();

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
	//connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
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