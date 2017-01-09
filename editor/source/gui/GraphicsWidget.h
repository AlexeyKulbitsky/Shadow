#ifndef SHADOW_EDITOR_GRAPHICS_WIDGET_INCLUDE
#define SHADOW_EDITOR_GRAPHICS_WIDGET_INCLUDE

#include <QWidget>
#include "../gizmo/Gizmo.h"
#include "../Picker.h"

namespace sh
{
	namespace video
	{
		class Driver;
	}

	namespace scene
	{
		class SceneManager;
	}

	class Entity;
}

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
	void Render();

signals:
	void EntitySelected(sh::Entity* entity);

protected:
	virtual void resizeEvent(QResizeEvent *e);
	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent * e);

private:
	sh::video::Driver* m_driver = nullptr;
	sh::scene::SceneManager* m_sceneManager = nullptr;
	Gizmo* m_gizmo = nullptr;
	Picker* m_picker = nullptr;
	bool m_leftButtonPressed = false;
};

#endif