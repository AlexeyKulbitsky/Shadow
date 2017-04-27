#ifndef SHADOW_EDITOR_EXPANDABLE_WIDGET_INCLUDE
#define SHADOW_EDITOR_EXPANDABLE_WIDGET_INCLUDE

#if 0

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

class ExpandableWidget : public QWidget 
{
	Q_OBJECT
public:
	explicit ExpandableWidget(const QString & title = "", const int animationDuration = 300, QWidget *parent = 0);
	void setContentLayout(QLayout & contentLayout);

private:
	QGridLayout mainLayout;
	QToolButton toggleButton;
	QFrame headerLine;
	QParallelAnimationGroup toggleAnimation;
	QScrollArea contentArea;
	int animationDuration{ 300 };
};

#endif

#endif