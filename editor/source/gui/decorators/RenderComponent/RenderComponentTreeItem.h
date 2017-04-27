#ifndef RENDER_COMPONENT_TREE_ITEM_INCLUDE
#define RENDER_COMPONENT_TREE_ITEM_INCLUDE

#if 0
#include <QList>
#include <QVariant>
#include <QVector>


class RenderComponentTreeItem : public QObject
{
	Q_OBJECT

public:
	enum class Type
	{
		LIST = 0,
		UNIFORM, 
		NONE
	};

    explicit RenderComponentTreeItem(const QVector<QVariant> &data, RenderComponentTreeItem *parent = 0);
    ~RenderComponentTreeItem();

    RenderComponentTreeItem *GetChild(int number);
    int childCount() const;
    int columnCount() const;
    QVariant GetData(int column) const;
    bool InsertChildren(int position, int count, int columns);
    bool InsertColumns(int position, int columns);
    RenderComponentTreeItem *GetParent();
    bool RemoveChildren(int position, int count);
    bool RemoveColumns(int position, int columns);
    int childNumber() const;
    bool SetData(int column, const QVariant &value);

	void SetType(Type type) { m_type = type; }
	Type GetType() const { return m_type; }

	void SetUserPointer(void* ptr) { m_userPointer = ptr; }
	void* GetUserPointer() { return m_userPointer; }

public slots:
	void SetDepthCompareFunction(int funcType);
	void SetUniformSingleFloat(double value);

private:
    QList<RenderComponentTreeItem*> m_childItems;
    QVector<QVariant> m_itemData;
    RenderComponentTreeItem *m_parentItem;
	Type m_type = Type::NONE;

	void* m_userPointer = nullptr;
};

#endif

#endif 