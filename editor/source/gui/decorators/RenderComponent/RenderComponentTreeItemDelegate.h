#ifndef QVOBJECTDELEGATE_H
#define QVOBJECTDELEGATE_H

#if 0
#include <QObject>
#include <QItemDelegate>

class RenderComponentTreeItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:

	RenderComponentTreeItemDelegate(QObject *parent);
	~RenderComponentTreeItemDelegate();

	void paint (QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const Q_DECL_OVERRIDE;	
	QSize sizeHint (const QStyleOptionViewItem & option, const QModelIndex & index) const Q_DECL_OVERRIDE;
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

protected:	
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;
	
	void PaintObject(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
	QRect m_buttonRect;
};

#endif

#endif // QVOBJECTDELEGATE_H
