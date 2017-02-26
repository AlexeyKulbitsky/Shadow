#ifndef SHADOW_EDITOR_GRAPHICS_WIDGET_INCLUDE
#define SHADOW_EDITOR_GRAPHICS_WIDGET_INCLUDE

#include <QWidget>
#include <Shadow.h>
#include "../gizmo/Gizmo.h"
#include "../gizmo/MoveGizmo.h"
#include "../gizmo/RotateGizmo.h"
#include "../gizmo/ScaleGizmo.h"


enum class EditMode
{
	MOVE = 0,
	ROTATE,
	SCALE
};

class GraphicsWidget : public QWidget
{
	Q_OBJECT;
public:
	GraphicsWidget(QWidget* parent);
	~GraphicsWidget();

	void Init();
	void Update();
	void Render();

public slots:
	void TransformActionChanged(QAction* action);

signals:
	void EntitySelected(sh::Entity* entity);

protected:
	virtual void resizeEvent(QResizeEvent *e);
	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent * e);
	virtual void wheelEvent(QWheelEvent * e);
	virtual void keyPressEvent(QKeyEvent * e);
	virtual void keyReleaseEvent(QKeyEvent * e);

private:
	sh::video::Driver* m_driver = nullptr;
	sh::scene::SceneManager* m_sceneManager = nullptr;

	Gizmo* m_gizmo = nullptr;

	Gizmo* m_defaultGizmo = nullptr;
	MoveGizmo* m_moveGizmo = nullptr;
	RotateGizmo* m_rotateGizmo = nullptr;
	ScaleGizmo* m_scaleGizmo = nullptr;

	sh::Entity* m_cameraTargetEntity = nullptr;
	bool m_leftButtonPressed = false;
};

#endif