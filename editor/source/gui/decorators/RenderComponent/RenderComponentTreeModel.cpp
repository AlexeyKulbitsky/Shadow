#include <QtWidgets>


#include "RenderComponentTreeItem.h"
#include "RenderComponentTreeModel.h"


RenderComponentTreeModel::RenderComponentTreeModel(const QStringList &headers, const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    m_rootItem = new RenderComponentTreeItem(rootData);
    setupModelData(data.split(QString("\n")), m_rootItem);
}

////////////////////////////////////////////////////////////////////////

RenderComponentTreeModel::RenderComponentTreeModel(const QStringList &headers, sh::scene::Model* model, QObject *parent)
	   : QAbstractItemModel(parent)
{
	QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    m_rootItem = new RenderComponentTreeItem(rootData);
    SetupModelData(model, m_rootItem);
}

////////////////////////////////////////////////////////////////////////

RenderComponentTreeModel::~RenderComponentTreeModel()
{
    delete m_rootItem;
}

////////////////////////////////////////////////////////////////////////

int RenderComponentTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return m_rootItem->columnCount();
}

////////////////////////////////////////////////////////////////////////

QVariant RenderComponentTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    RenderComponentTreeItem *item = getItem(index);

    return item->GetData(index.column());
}

////////////////////////////////////////////////////////////////////////

Qt::ItemFlags RenderComponentTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

////////////////////////////////////////////////////////////////////////

RenderComponentTreeItem *RenderComponentTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        RenderComponentTreeItem *item = static_cast<RenderComponentTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}

////////////////////////////////////////////////////////////////////////

QVariant RenderComponentTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->GetData(section);

    return QVariant();
}

////////////////////////////////////////////////////////////////////////

QModelIndex RenderComponentTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    RenderComponentTreeItem *parentItem = getItem(parent);

    RenderComponentTreeItem *childItem = parentItem->GetChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = m_rootItem->InsertColumns(position, columns);
    endInsertColumns();

    return success;
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    RenderComponentTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->InsertChildren(position, rows, m_rootItem->columnCount());
    endInsertRows();

    return success;
}

////////////////////////////////////////////////////////////////////////

QModelIndex RenderComponentTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    RenderComponentTreeItem *childItem = getItem(index);
    RenderComponentTreeItem *parentItem = childItem->GetParent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = m_rootItem->RemoveColumns(position, columns);
    endRemoveColumns();

    if (m_rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    RenderComponentTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->RemoveChildren(position, rows);
    endRemoveRows();

    return success;
}

////////////////////////////////////////////////////////////////////////

int RenderComponentTreeModel::rowCount(const QModelIndex &parent) const
{
    RenderComponentTreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    RenderComponentTreeItem *item = getItem(index);
    bool result = item->SetData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

////////////////////////////////////////////////////////////////////////

bool RenderComponentTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = m_rootItem->SetData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

////////////////////////////////////////////////////////////////////////

void RenderComponentTreeModel::setupModelData(const QStringList &lines, RenderComponentTreeItem *parent)
{
    QList<RenderComponentTreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) 
	{
        int position = 0;
        while (position < lines[number].length()) 
		{
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) 
		{
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) 
			{
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) 
				{
                    parents << parents.last()->GetChild(parents.last()->childCount()-1);
                    indentations << position;
                }
            } 
			else 
			{
                while (position < indentations.last() && parents.count() > 0) 
				{
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            RenderComponentTreeItem *parent = parents.last();
            parent->InsertChildren(parent->childCount(), 1, m_rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->GetChild(parent->childCount() - 1)->SetData(column, columnData[column]);
        }

        ++number;
    }
}

////////////////////////////////////////////////////////////////////////

void RenderComponentTreeModel::SetupModelData(sh::scene::Model* model, RenderComponentTreeItem *parent)
{
	QList<RenderComponentTreeItem*> parents;
	parents << parent;
	size_t meshesCount = model->GetMeshesCount();

	for (size_t i = 0; i < meshesCount; ++i)
	{
		sh::scene::MeshPtr mesh = model->GetMesh(i);
		sh::video::Material* mat = mesh->GetMaterial();
		sh::video::RenderTechnique* rt = mat->GetRenderTechnique();
		QVariant data(rt->GetName().c_str());

		RenderComponentTreeItem *parent = parents.last();
        parent->InsertChildren(parent->childCount(), 1, 2);
		
		parent->GetChild(parent->childCount() - 1)->SetData(0, QVariant("RenderTechnique"));
		parent->GetChild(parent->childCount() - 1)->SetData(1, data);

		size_t passesCount = mat->GetRenderPipelinesCount();
		for (size_t passIndex = 0; passIndex < passesCount; ++passIndex)
		{
			RenderComponentTreeItem * techniqueParent = parent->GetChild(parent->childCount() - 1);
			
			sh::video::RenderPipeline* pipeline = mat->GetRenderPipeline(passIndex);
			QVariant passName(pipeline->GetName().c_str());

			techniqueParent->InsertChildren(techniqueParent->childCount(), 1, 2);

			techniqueParent->GetChild(techniqueParent->childCount() - 1)->SetData(0, QVariant("RenderPass"));
			techniqueParent->GetChild(techniqueParent->childCount() - 1)->SetData(1, passName);


			RenderComponentTreeItem * passParent = techniqueParent->GetChild(techniqueParent->childCount() - 1);
			sh::video::DepthStencilState* depthStencilState = pipeline->GetDepthStencilState();
			sh::video::RasterizationState* rasterizationState = pipeline->GetRasterizationState();
			sh::video::BlendingState* blendingState = pipeline->GetBlendingState();

			// Depthstencil state
			passParent->InsertChildren(passParent->childCount(), 1, 1);
			passParent->GetChild(passParent->childCount() - 1)->SetData(0, QVariant("DepthStencilState"));
			RenderComponentTreeItem * depthStencilParent = passParent->GetChild(passParent->childCount() - 1);

			depthStencilParent->InsertChildren(depthStencilParent->childCount(), 1, 2);
			RenderComponentTreeItem * tempChild = depthStencilParent->GetChild(depthStencilParent->childCount() - 1);
			tempChild->SetData(0, QVariant("Depth Test"));
			tempChild->SetData(1, depthStencilState->enableDepthTest ? QVariant("On") : QVariant("Off"));

			depthStencilParent->InsertChildren(depthStencilParent->childCount(), 1, 2);
			tempChild = depthStencilParent->GetChild(depthStencilParent->childCount() - 1);
			tempChild->SetType(RenderComponentTreeItem::Type::LIST);
			tempChild->SetData(0, QVariant("Depth Compare Func"));
			tempChild->SetData(1, sh::video::g_compareFunctionMap[(size_t)depthStencilState->depthCompareFunction]);
			tempChild->SetUserPointer((void*)(&(depthStencilState->depthCompareFunction)));
			
			depthStencilParent->InsertChildren(depthStencilParent->childCount(), 1, 2);
			tempChild = depthStencilParent->GetChild(depthStencilParent->childCount() - 1);
			tempChild->SetType(RenderComponentTreeItem::Type::LIST);
			tempChild->SetData(0, QVariant("Depth Write Mask"));
			tempChild->SetData(1, sh::video::g_writeMaskMap[(size_t)depthStencilState->depthWriteMask]);		



			// Rasterizer state
			passParent->InsertChildren(passParent->childCount(), 1, 1);
			passParent->GetChild(passParent->childCount() - 1)->SetData(0, QVariant("RasterizerState"));
			RenderComponentTreeItem * rasterizerParent = passParent->GetChild(passParent->childCount() - 1);

			rasterizerParent->InsertChildren(rasterizerParent->childCount(), 1, 2);
			tempChild = rasterizerParent->GetChild(rasterizerParent->childCount() - 1);
			tempChild->SetData(0, QVariant("Cull Face"));
			tempChild->SetData(1, sh::video::g_cullFaceMap[(size_t)rasterizationState->cullFace]);

			rasterizerParent->InsertChildren(rasterizerParent->childCount(), 1, 2);
			tempChild = rasterizerParent->GetChild(rasterizerParent->childCount() - 1);
			tempChild->SetData(0, QVariant("Front Face"));
			tempChild->SetData(1, sh::video::g_frontFaceMap[(size_t)rasterizationState->frontFace]);
			
			rasterizerParent->InsertChildren(rasterizerParent->childCount(), 1, 2);
			tempChild = rasterizerParent->GetChild(rasterizerParent->childCount() - 1);
			tempChild->SetData(0, QVariant("Fill Mode"));
			tempChild->SetData(1, sh::video::g_fillModeMap[(size_t)rasterizationState->fillMode]);
			
			// Uniforms
			passParent->InsertChildren(passParent->childCount(), 1, 1);
			passParent->GetChild(passParent->childCount() - 1)->SetData(0, QVariant("Uniforms"));
			RenderComponentTreeItem * uniformParent = passParent->GetChild(passParent->childCount() - 1);

			sh::video::UniformBuffer* uniformBuffer = pipeline->GetUniformBuffer();
			size_t uniformsCount = uniformBuffer->GetUniformsCount();
			for (size_t uniformIdx = 0; uniformIdx < uniformsCount; ++uniformIdx)
			{
				sh::video::Uniform* uniform = uniformBuffer->GetUniform(uniformIdx);
				if (uniform->GetType() == sh::video::Uniform::Type::FLOAT)
				{
					uniformParent->InsertChildren(uniformParent->childCount(), 1, 2);
					tempChild = uniformParent->GetChild(uniformParent->childCount() - 1);
					tempChild->SetData(0, QVariant(uniform->GetName().c_str()));
					tempChild->SetData(1, QVariant(uniform->Get<float>()));
					tempChild->SetUserPointer((void*)uniform);
					tempChild->SetType(RenderComponentTreeItem::Type::UNIFORM);
				}
			}
		}
           
	}
	/*
	 QList<RenderComponentTreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) 
	{
        int position = 0;
        while (position < lines[number].length()) 
		{
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) 
		{
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) 
			{
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) 
				{
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } 
			else 
			{
                while (position < indentations.last() && parents.count() > 0) 
				{
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            RenderComponentTreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, m_rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }

	*/
}

////////////////////////////////////////////////////////////////////////


