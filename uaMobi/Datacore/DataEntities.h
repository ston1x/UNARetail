#pragma once
#include "BarcodeEntity.h"
#include "PricedBarcodeEntity.h"
#include "ShortBarcodeEntity.h"
#include <QAbstractListModel>




class DataEntityListModel : public QAbstractListModel
	// This data model is used for fully dynamical data entity model. Use right delegate to show data in view.
	// You can place in this model even polymorthic data, just install suitable delegate for it.
{
	Q_OBJECT
protected:
	// real data storage
	QVector<Entity> innerList;
public:
	// Roles for data entity operations
	enum ExtendedRoles {
		// Search role returns polymorthic pointer to call ->compare
		SearchRole = Qt::UserRole + 1,
		// DataCopyRole returns full copy of an object to avoid changing model
		DataCopyRole,
		DirectAccessRole
	};

	using QAbstractListModel::QAbstractListModel;
	DataEntityListModel(const QVector<Entity>& ents, QWidget* parent = nullptr);
	// Inherited from QAbstractListModel
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	// dataoperation methods
	//post-constructor
	void insertData(const QVector<Entity>& data);
	// deletes data entity in the model by it's index or pointer
	void removeDataEntity(const QModelIndex&);
	void removeDataEntity(Entity);
	// replaces data entity by it's id. This method is used only if you have a copy with the same id and other data.
	void replaceDataEntity(Entity);

	void appendDataEntity(Entity);
	// empties model
	void reset();
public slots:
	//If index was valid - emits signal with pointer to clicked entity.
	void mapClickToEntity(const QModelIndex& index);
signals:
	// delivers pointer to data entity. Warning - changing this entity by pointer will affect model.
	void dataEntityClicked(Entity);
};

extern const QHash<barcodeUtil::barcodetypes, Entity> entityLinker;



template <class DataEntity>
std::shared_ptr<DataEntity> upcastEntity(std::shared_ptr<AbsEntity> e)
{
	DataEntity te;
	if (e->myType() == te.myType())
	{
		return std::static_pointer_cast<DataEntity>(e);
	}
	return std::shared_ptr<DataEntity>();
}
