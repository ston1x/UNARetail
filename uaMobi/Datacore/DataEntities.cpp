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
	if (role == Qt::DisplayRole || role == SearchRole)
	{
		return QVariant(innerList.at(index.row())->getName());
	}
	else if (role == DataCopyRole)
	{
		QVariant temp;
		temp.setValue<Entity>(Entity(innerList.at(index.row())->clone()));
		return temp;
	}
	else if (role == DirectAccessRole)
	{
		QVariant temp;
		temp.setValue<Entity>(innerList.at(index.row()));
		return temp;
	}
	return QVariant();
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
	innerList.removeAt(mindex.row());
	endRemoveRows();
}

void DataEntityListModel::removeDataEntity(Entity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if ((*innerList.at(i)) == (e))
		{
			beginRemoveRows(createIndex(i, 0), i, i);
			innerList.removeAt(i);
			endRemoveRows();
		}
	}
}

void DataEntityListModel::replaceDataEntity(Entity e)
{
	for (int i = 0; i < innerList.count(); ++i)
	{
		if ((*innerList.at(i)) == e)
		{
			innerList[i] = e;
		}
	}
}

void DataEntityListModel::appendDataEntity(Entity e)
{
	beginInsertRows(QModelIndex(), innerList.count() - 1, innerList.count());
	innerList << e;
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

const QHash<barcodeUtil::barcodetypes, Entity> entityLinker
{
	{ barcodeUtil::barcodetypes::uniformBc, Entity(new BarcodeEntity())},
	{ barcodeUtil::barcodetypes::pricedBc, Entity(new PricedBarcodeEntity())},
	{ barcodeUtil:: barcodetypes::shortBc, Entity(new ShortBarcodeEntity())}
};