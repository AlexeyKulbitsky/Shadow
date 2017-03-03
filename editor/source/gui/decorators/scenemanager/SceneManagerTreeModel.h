#ifndef SCENE_MANAGER_TREEMODEL_H
#define SCENE_MANAGER_TREEMODEL_H

#if 0

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include <Shadow.h>

class SceneManagerTreeItem;

class SceneManagerTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    SceneManagerTreeModel(const QStringList &headers, const QString &data, QObject *parent = 0);
    ~SceneManagerTreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;


    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

private:
    void setupModelData(const QStringList &lines, SceneManagerTreeItem *parent);
	void SetupModelData(sh::scene::Model* model, SceneManagerTreeItem *parent);

public:	
	SceneManagerTreeItem *getItem(const QModelIndex &index) const;

    SceneManagerTreeItem *m_rootItem;
};

#endif

#endif