#include "RenderComponentTreeItemDelegate.h"
#include "RenderComponentTreeModel.h"
#include "RenderComponentTreeItem.h"
#include <QPainter>
#include <QApplication>
#include <QComboBox>
#include <QDoublespinbox>

#include <video\Uniform.h>
#include <video\BlendingState.h>
#include <video\DepthStencilState.h>
#include <video\RasterizationState.h>

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

						for (size_t i = 0; i < (size_t)sh::video::CompareFunction::COUNT; ++i)
						{
							list << sh::video::g_compareFunctionMap[i];
						}
						comboBox->addItems(list);

						connect(comboBox, SIGNAL(currentIndexChanged(int)), item, SLOT(SetDepthCompareFunction(int)));
						return comboBox;
					}			
				}
					break;
				case RenderComponentTreeItem::Type::UNIFORM:
				{				
					sh::video::Uniform* uniform = (sh::video::Uniform*)item->GetUserPointer();
					switch (uniform->GetType())
					{
						case sh::video::Uniform::Type::FLOAT:
						{
							QDoubleSpinBox* spinBox = new QDoubleSpinBox(parent);
							float value = uniform->Get<float>();							
							spinBox->setMaximum(1.0);
							spinBox->setMinimum(0.0);
							spinBox->setSingleStep(0.05);
							spinBox->setValue(value);					
							connect(spinBox, SIGNAL(valueChanged(double)), item, SLOT(SetUniformSingleFloat(double)));
							return spinBox;
						}
							break;
						default:
							break;
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

	