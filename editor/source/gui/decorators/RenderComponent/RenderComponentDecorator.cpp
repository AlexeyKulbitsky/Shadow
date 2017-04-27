#include "RenderComponentDecorator.h"

#if 0
#include <QTreeView>
#include "RenderComponentTreeModel.h"
#include "RenderComponentTreeItemDelegate.h"


RenderComponentDecorator::RenderComponentDecorator()
{
}

///////////////////////////////////////////////////

RenderComponentDecorator::~RenderComponentDecorator()
{
}

///////////////////////////////////////////////////

ExpandableWidget* RenderComponentDecorator::GetParametersWidget()
{
	m_parametersWidget = new ExpandableWidget("Render");

	QStringList headers;
    headers << tr("Title") << tr("Description");

	//TreeModel* model = new TreeModel(headers, QString());
	RenderComponentTreeModel* model = new RenderComponentTreeModel(headers, m_model.get());
	QTreeView* view = new QTreeView();
	view->setModel(model);
	RenderComponentTreeItemDelegate* itemDelegate = new RenderComponentTreeItemDelegate(view);
	view->setItemDelegate(itemDelegate);

	QVBoxLayout* mainLayout = new QVBoxLayout();

	mainLayout->addWidget(view);

	m_parametersWidget->setContentLayout(*mainLayout);

	return m_parametersWidget;
}

///////////////////////////////////////////////////

#endif

