#ifndef SHADOW_EDITOR_GRAPHICS_WIDGET_INCLUDE
#define SHADOW_EDITOR_GRAPHICS_WIDGET_INCLUDE

#include <QWidget>

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
}

class GraphicsWidget : public QWidget
{
	Q_OBJECT;
public:
	GraphicsWidget(QWidget* parent);
	~GraphicsWidget();

	void Init();
	void Render();

protected:
	virtual void resizeEvent(QResizeEvent *e);
	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent * e);

private:
	sh::video::Driver* m_driver = nullptr;
	sh::scene::SceneManager* m_sceneManager = nullptr;
};

#endif