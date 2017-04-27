#include "RenderComponentTreeItemDelegate.h"
#if 0
#include "RenderComponentTreeModel.h"
#include "RenderComponentTreeItem.h"
#include <QPainter>
#include <QApplication>
#include <QComboBox>
#include <QDoublespinbox>

#include <Shadow.h>

RenderComponentTreeItemDelegate::RenderComponentTreeItemDelegate(QObject *parent)
	: QItemDelegate(parent)
{

}

///////////////////////////////////////////////////////

RenderComponentTreeItemDelegate::~RenderComponentTreeItemDelegate()
{

}

///////////////////////////////////////////////////////

void RenderComponentTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QItemDelegate::paint(painter, option, index);
	//PaintObject(painter, option, index);
}

///////////////////////////////////////////////////////

QSize RenderComponentTreeItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QItemDelegate::sizeHint(option, index);
}

///////////////////////////////////////////////////////

QWidget* RenderComponentTreeItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{	
	const RenderComponentTreeModel* model =  qobject_cast<const RenderComponentTreeModel*>(index.model());
	if (model)
	{
		RenderComponentTreeItem *item = model->getItem(index);
		if (item)
		{
			RenderComponentTreeItem::Type type = item->GetType();

			switch (type)
			{
				case RenderComponentTreeItem::Type::LIST:
				{				
					if (item->GetData(0).toString() == QString("Depth Compare Func"))
					{
						QComboBox* comboBox = new QComboBox(parent);
						QStringList list;

						for (size_t i = 0; i < (size_t)sh::CompareFunction::COMP_FUNC_COUNT; ++i)
						{
							list << sh::g_compareFunctionMap[i];
						}
						comboBox->addItems(list);

						connect(comboBox, SIGNAL(currentIndexChanged(int)), item, SLOT(SetDepthCompareFunction(int)));
						return comboBox;
					}			
				}
					break;
			}
					
		}
	}

	return QItemDelegate::createEditor(parent, option, index);
}

///////////////////////////////////////////////////////
	
bool RenderComponentTreeItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	return QItemDelegate::editorEvent(event, model, option, index);
}

///////////////////////////////////////////////////////

void RenderComponentTreeItemDelegate::PaintObject(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//painter->restore();
	painter->save();

	QStyle * style = QApplication::style();
	QStyleOptionButton opt;
	
	opt.rect  = option.rect;
	opt.state |= QStyle::State_Active;
	opt.state |= QStyle::State_Enabled;	
	opt.text = index.data(Qt::DisplayRole).toString();
	
	if(style)
		style->drawControl(QStyle::CE_PushButton, &opt, painter );
	
}

///////////////////////////////////////////////////////

#endif
	