#include "RenderComponentTreeItem.h"


#if 0
#include <QStringList>
#include <Shadow.h>


RenderComponentTreeItem::RenderComponentTreeItem(const QVector<QVariant> &data, RenderComponentTreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

////////////////////////////////////////////////////////////////////////

RenderComponentTreeItem::~RenderComponentTreeItem()
{
    qDeleteAll(m_childItems);
}

////////////////////////////////////////////////////////////////////////

RenderComponentTreeItem *RenderComponentTreeItem::GetChild(int number)
{
    return m_childItems.value(number);
}

////////////////////////////////////////////////////////////////////////

int RenderComponentTreeItem::childCount() const
{
    return m_childItems.count();
}

////////////////////////////////////////////////////////////////////////

int RenderComponentTreeItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<RenderComponentTreeItem*>(this));

    return 0;
}

////////////////////////////////////////////////////////////////////////

int RenderComponentTreeItem::columnCount() const
{
    return m_itemData.count();
}

////////////////////////////////////////////////////////////////////////

QVariant RenderComponentTreeItem::GetData(int column) const
{
    return m_itemData.value(column);
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeItem::InsertChildren(int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) 
	{
        QVector<QVariant> data(columns);
        RenderComponentTreeItem *item = new RenderComponentTreeItem(data, this);
        m_childItems.insert(position, item);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeItem::InsertColumns(int position, int columns)
{
    if (position < 0 || position > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.insert(position, QVariant());

    foreach (RenderComponentTreeItem *child, m_childItems)
        child->InsertColumns(position, columns);

    return true;
}

////////////////////////////////////////////////////////////////////////

RenderComponentTreeItem *RenderComponentTreeItem::GetParent()
{
    return m_parentItem;
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeItem::RemoveChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeItem::RemoveColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.remove(position);

    foreach (RenderComponentTreeItem *child, m_childItems)
        child->RemoveColumns(position, columns);

    return true;
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeItem::SetData(int column, const QVariant &value)
{
    if (column < 0 || column >= m_itemData.size())
        return false;

    m_itemData[column] = value;
    return true;
}

////////////////////////////////////////////////////////////////////////

void RenderComponentTreeItem::SetDepthCompareFunction(int funcType)
{
	sh::CompareFunction *value = (sh::CompareFunction*)m_userPointer;
	*value = (sh::CompareFunction)funcType;
}

////////////////////////////////////////////////////////////////////////

void RenderComponentTreeItem::SetUniformSingleFloat(double value)
{
}

////////////////////////////////////////////////////////////////////////

#endif

