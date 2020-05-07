#include "DataEntities.h"
#include <QWidget>

DataEntityListModel::DataEntityListModel(const QVector<Entity>& entities, QWidget* parent)
	: QAbstractListModel(parent), innerList(entities)
{
}

int DataEntityListModel::rowCount(const QModelIndex& /*parent*/) const
{
	return innerList.count();
}

QVariant DataEntityListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= rowCount())
		return QVariant();
	switch (role)
	{
	case Qt::DisplayRole:
	case SearchRole:
		return QVariant(innerList.at(index.row())->getName());
	case DataCopyRole:
	{
		QVariant temp;
		temp.setValue<Entity>(Entity(innerList.at(index.row())->clone()));
		return temp;
	}
	case  DirectAccessRole:
	{
		QVariant temp;
		temp.setValue<Entity>(innerList.at(index.row()));
		return temp;
	}
	case ReversedDARole:
	{
		QVariant temp;
		temp.setValue<Entity>(innerList.at(innerList.count() - 1 - index.row()));
		return temp;
	}
	default:
		return QVariant();
	}
}

QVariant DataEntityListModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
{
	return QVariant();
}

void DataEntityListModel::insertData(const QVector<Entity>& data)
{
	beginResetModel();
	innerList.clear();
	innerList << data;
	endResetModel();
}

void DataEntityListModel::removeDataEntity(const QModelIndex& mindex)
{
	if (!mindex.isValid())
		return;
	beginRemoveRows(mindex, mindex.row(), mindex.row());
    innerList.remove(mindex.row());
	endRemoveRows();
}

void DataEntityListModel::removeDataEntity(Entity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if ((*innerList.at(i)) == (e))
		{
			beginRemoveRows(createIndex(i, 0), i, i);
            innerList.remove(i);
			endRemoveRows();
		}
	}
}

void DataEntityListModel::replaceDataEntity(Entity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if ((*innerList.at(i)).getId() == e->getId())
		{
			innerList[i] = e;
		}
	}
}

void DataEntityListModel::addToDataEntity(Entity e, int role)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if ((*innerList.at(i)).getName() == e->getName())
		{
			innerList[i]->setEnumerable(role, e->getEnumerable(role) + innerList[i]->getEnumerable(role));
			dataChanged(createIndex(i - 1, 0), createIndex(i, 0));
			return;
		}
	}
	appendDataEntity(e);
}

void DataEntityListModel::appendDataEntity(Entity e)
{
	beginInsertRows(QModelIndex(), (innerList.count() - 1) >= 0 ? innerList.count() - 1 : 0, innerList.count());
	innerList << Entity(e->clone());
	endInsertRows();
}

void DataEntityListModel::reset()
{
	beginResetModel();
	innerList.clear();
	endResetModel();
}

void DataEntityListModel::mapClickToEntity(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	emit dataEntityClicked(innerList.at(index.row()));
}
QHash<barcodeUtil::barcodetypes, Entity> _initentityLinker()
{
    QHash<barcodeUtil::barcodetypes, Entity> t;
    t.insert(barcodeUtil::barcodetypes::uniformBc, Entity(new BarcodeEntity()));
    t.insert(barcodeUtil::barcodetypes::pricedBc, Entity(new PricedBarcodeEntity()));
    t.insert( barcodeUtil:: barcodetypes::shortBc, Entity(new ShortBarcodeEntity()));
    return t;
}

const QHash<barcodeUtil::barcodetypes, Entity> entityLinker(_initentityLinker());

