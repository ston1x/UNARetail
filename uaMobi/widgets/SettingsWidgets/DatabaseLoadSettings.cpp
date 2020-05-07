#include "DatabaseLoadSettings.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include <QMessageBox>
#include "Datacore/ShortBarcodeEntity.h"
#include <QMimeData>
#include "widgets/ExtendedDelegates/ExtraSelectionDelegate.h"
#include "debugtrace.h"
#include <qapplication.h>
#include "dataproviders/sqldataprovider.h"
QVector<int> DatabaseLoadSettings::extractInsertionIndexes()
{
	
	return model->getSelectedIndexes().toVector();
}

QVector<indexedString> DatabaseLoadSettings::_extractFields()
{
	QVector<indexedString> v;
	if (AppSettings->deserializationPoints.isEmpty() || AppSettings->deserializationPoints.count() < ShortBarcodeEntity::getFieldNames().count())
	{
		const QStringList & li = ShortBarcodeEntity::getFieldNames();
		for (int i = 0; i < li.count(); ++i)
		{
			v.push_back(indexedString(li.at(i), i));
		}
		return v;
	}
	else
	{
		for (int i = 0; i < AppSettings->deserializationPoints.count(); ++i)
		{
			if (AppSettings->deserializationPoints.at(i) < 0)
			{
				switch (AppSettings->deserializationPoints.at(i))
				{
				case -2:
					v << indexedString(tr("Skip"), -2);
					break;
				case -3:
					v << indexedString(tr("Join"), -3);
					break;
				}
			}
			else
			{
				if (AppSettings->deserializationPoints.at(i) < ShortBarcodeEntity::getFieldNames().count())
				{
					v << indexedString(ShortBarcodeEntity::getFieldNames().at(AppSettings->deserializationPoints.at(i)), AppSettings->deserializationPoints.at(i));
				}
			}
		}
		return v;
	}
}

QList<int> DatabaseLoadSettings::_extractPoints()
{
	QList<int> v;
	for (int i = 0; i < model->rowCount(); ++i)
	{
		v << model->data(i).index;
	}
	return v;
}

void DatabaseLoadSettings::_addToBarcode(int index, ShortBarcode & shb,const QString& parsedColumn)
{
	switch (index)
	{
	case 0:
		shb->GUID = parsedColumn.toLongLong();
		break;
	case 1:
		shb->barcode += parsedColumn;
		break;
	case 2:
		shb->code = parsedColumn.toInt();
		break;
	case 3:
		shb->info += parsedColumn;
		break;
	case 4:
		shb->count += parsedColumn;
		break;
	case -2:
		break;
	default:
		break;
	}
}

DatabaseLoadSettings::DatabaseLoadSettings(QWidget* parent)
	:QWidget(parent),
	mainLayout(new QVBoxLayout(this)),
	model(new IndexedListModel(this)),
	databasePathInfo(new QLabel(this)),
	databasePath(new QLineEdit(this)),
	separatorCodeInfo(new QLabel(this)),
	separatorCode(new QSpinBox(this)),
	fieldSelector(new QListView(this)),
	controlLayout(new QHBoxLayout(this)),
	deleteButton(new MegaIconButton(this)),
	addButton(new MegaIconButton(this)),
	joinButton(new MegaIconButton(this)),
lowerLayout(new QHBoxLayout(this)),
dropDatabaseButton(new MegaIconButton(this)),
fromFileLoad(new MegaIconButton(this))
{
	setLayout(mainLayout);
	mainLayout->addWidget(databasePathInfo);
	databasePathInfo->setText(tr("local database path"));
	databasePathInfo->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(databasePath);
	separatorCodeInfo->setText(tr("Separator code"));
	separatorCodeInfo->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(separatorCodeInfo);
	mainLayout->addWidget(separatorCode);
	mainLayout->addWidget(fieldSelector);
	
	mainLayout->addItem(controlLayout);
	controlLayout->addWidget(addButton);
	controlLayout->addWidget(deleteButton);
	controlLayout->addWidget(joinButton);
	mainLayout->addItem(lowerLayout);
	lowerLayout->addWidget(dropDatabaseButton);
	lowerLayout->addWidget(fromFileLoad);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	fromFileLoad->setText(tr("Load from local"));
	fromFileLoad->setIcon(QIcon(":/res/viaFile.png"));
	fromFileLoad->setStyleSheet(DOWNLOAD_BUTTONS_STYLESHEET);
	dropDatabaseButton->setText(tr("Drop DB"));
	dropDatabaseButton->setIcon(QIcon(":/res/deleteData.png"));
	dropDatabaseButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);


	databasePath->setText(AppSettings->localDatabase);
	addButton->setText("+");
	addButton->setIcon(QIcon(":/res/plus.png"));
	deleteButton->setIcon(QIcon(":/res/minus.png"));
	joinButton->setIcon(QIcon(":/res/pencil.png"));
	deleteButton->setText("-");
	joinButton->setText("&");


	model->insertData(_extractFields());
	fieldSelector->setModel(model);
	fieldSelector->setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
	fieldSelector->setDragEnabled(true);
	fieldSelector->setDropIndicatorShown(true);
	fieldSelector->setSelectionMode(QListView::SingleSelection);
	fieldSelector->setResizeMode(QListView::ResizeMode::Adjust);
	fieldSelector->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	separatorCode->setValue(AppSettings->separatorCode.unicode());
	model->selectIndexes(AppSettings->deserializationOrder);
	fieldSelector->setItemDelegate(new ExtraSelectionDelegate(this));
#ifdef QT_VERSION5X
	QObject::connect(fromFileLoad, &MegaIconButton::clicked, this, &DatabaseLoadSettings::loadDatabaseFromFile);
	QObject::connect(deleteButton, &MegaIconButton::clicked, this, &DatabaseLoadSettings::deleteEmpty);
	QObject::connect(addButton, &MegaIconButton::clicked, this, &DatabaseLoadSettings::addEmpty);
	QObject::connect(joinButton, &MegaIconButton::clicked, this, &DatabaseLoadSettings::addJoin);
	QObject::connect(fieldSelector, &QListView::doubleClicked, model, &IndexedListModel::selectItem);
	QObject::connect(dropDatabaseButton, &MegaIconButton::clicked, this, &DatabaseLoadSettings::dropDB);
#else
	QObject::connect(fromFileLoad, SIGNAL(clicked()), this, SLOT(loadDatabaseFromFile()));
	QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEmpty()));
	QObject::connect(addButton, SIGNAL(clicked()), this, SLOT(addEmpty()));
	QObject::connect(joinButton, SIGNAL(clicked()), this, SLOT(addJoin()));
	QObject::connect(fieldSelector, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(selectItem(QModelIndex)));
	QObject::connect(dropDatabaseButton, SIGNAL(clicked()), this, SLOT(dropDB()));
#endif
}

void DatabaseLoadSettings::extractAndSave()
{
	AppSettings->localDatabase = databasePath->text();
	AppSettings->separatorCode = separatorCode->value();
	QList<int> indexes;
	AppSettings->deserializationOrder = model->getSelected();
	AppSettings->deserializationPoints = _extractPoints();
}

void DatabaseLoadSettings::retranslate()
{
	databasePathInfo->setText(tr("local database path"));
	separatorCodeInfo->setText(tr("Separator code"));
	fromFileLoad->setText(tr("Load from local"));
}

void DatabaseLoadSettings::addEmpty()
{
	model->pushEmpty();
}

void DatabaseLoadSettings::deleteEmpty()
{
	model->popEmpty();
}

void DatabaseLoadSettings::addJoin()
{
	model->pushJoin();
}

void DatabaseLoadSettings::dropDB()
{
	AppData->recreateDownloadTable();
}



void DatabaseLoadSettings::loadDatabaseFromFile()
{
	if (databasePath->text().isEmpty())
	{
		return;
	}
	else
	{
		QFile f(databasePath->text());
		f.open(QIODevice::ReadOnly);
		if (!f.isOpen() || !f.isReadable())
			return;
		QVector<int> indexes = extractInsertionIndexes();
		QTextStream in(&f);
		ShortBarcode shb(new ShortBarcodeEntity());
		while (!in.atEnd())
		{
			QString line = in.readLine();
			shb->clear();
			QStringList splitted = line.split(QChar(separatorCode->value()));
			if (splitted.count() != indexes.count())
			{
				continue;
			}
			for (int i = 0; i < indexes.count(); ++i)
			{
				switch (indexes.at(i))
				{
				case -3:
				{
					int j = i - 1;
					while (j > -1 && indexes.at(j) < 0)
					{
						--j;
					}
					if (j >= 0)
						_addToBarcode(indexes.at(j), shb, splitted.at(i));
				}
				default:
					_addToBarcode(indexes.at(i), shb, splitted.at(i));
					break;
				}
			}
			AppData->pushIntoDownloaded(*shb);
			qApp->processEvents();
		}
		QMessageBox::warning(this, tr("Success"), tr("File read succesfully"));
		f.close();
	}
}

int IndexedListModel::rowCount(const QModelIndex& parent) const
{
	return innerList.count();
}

bool IndexedListModel::insertRows(int row, int count, const QModelIndex& parent)
{
	QVector<indexedString> temp;
	temp.reserve(innerList.count() + count);
	for (int i = 0; i <= row ; ++i)
	{
		temp << innerList.at(i);
	}
	for (int i = 0; i < count; ++i)
	{
		temp << indexedString();
	}
	for (int i = row; i < innerList.count(); ++i)
	{
		temp << innerList.at(i);
	}
	innerList = temp;
	return  true;
}

bool IndexedListModel::insertColumns(int column, int count, const QModelIndex& parent)
{
	return true;
}



Qt::DropActions IndexedListModel::supportedDragActions() const
{
	return Qt::DropAction::TargetMoveAction | Qt::DropAction::CopyAction | Qt::DropAction::MoveAction | Qt::DropAction::LinkAction;
}

bool IndexedListModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	if (data == Q_NULLPTR)
		return false;
	if (data->hasText())
	{
		bool ok = false;
		int indexForSwap = data->text().toInt(&ok);
		if (!ok)
			return false;
		std::swap(innerList[parent.row()], innerList[indexForSwap]);
		return true;
	}
	return false;
}

bool IndexedListModel::removeRows(int row, int count, const QModelIndex& parent)
{
	
	return false;
}

bool IndexedListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (index.isValid())
	{
		switch (role)
		{
		case Qt::DisplayRole:
			innerList[index.row()].string = value.toString();
			break;
		case Qt::EditRole:
			innerList[index.row()] = value.value<indexedString>();
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}

bool IndexedListModel::setItemData(const QModelIndex& index, const QMap<int, QVariant>& roles)
{
	QMap<int, QVariant>::const_iterator begin = roles.begin();
	int ind = index.row();
	while (begin != roles.end())
	{
		if (ind >= innerList.count())
			return false;
		switch (begin.key())
		{
		case Qt::DisplayRole:
			innerList[ind].string = begin->toString();
			break;
		case Qt::EditRole:
			innerList[ind] = begin->value<indexedString>();
			break;
		default:
			break;
		}
		++ind;
		++begin;
	}
	return true;
}

QMimeData* IndexedListModel::mimeData(const QModelIndexList& indexes) const
{
	if (indexes.isEmpty())
		return Q_NULLPTR;
	QString out = QString::number(indexes.first().row());
	QMimeData* data = new QMimeData();
	data->setText(out);
	return data;
}

bool IndexedListModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
	if (data->hasText())
		return true;
	return false;
}

Qt::ItemFlags IndexedListModel::flags(const QModelIndex& index) const
{
	if (index.isValid())
	{
		return Qt::ItemFlag::ItemIsDragEnabled | Qt::ItemFlag::ItemIsDropEnabled | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEnabled;
	}
	return Qt::ItemFlags();
}

QStringList IndexedListModel::mimeTypes() const
{
	return QStringList("text/plain");
}

Qt::DropActions IndexedListModel::supportedDropActions() const
{
	
	return Qt::DropAction::TargetMoveAction | Qt::DropAction::CopyAction | Qt::DropAction::MoveAction | Qt::DropAction::LinkAction;

}

QVariant IndexedListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= rowCount())
		return QVariant();
	switch (role)
	{
	case Qt::DisplayRole:
		return QVariant(innerList.at(index.row()).string);
	case SelectionRole:
		return QVariant(innerList.at(index.row()).isSelected);
	default:
		return QVariant();
	}
}

QVariant IndexedListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void IndexedListModel::insertData(const QStringList& data)
{
	for (int i = 0; i < data.count(); ++i)
	{
		innerList << indexedString(data.at(i), i);
	}
}

void IndexedListModel::insertData(const QVector<indexedString>& v)
{
	innerList = v;
}

void IndexedListModel::pushEmpty()
{
	beginInsertRows(createIndex(innerList.count() - 1, 0), innerList.count() - 1, innerList.count());
	innerList << indexedString(tr("Skip"), -2);
	endInsertRows();
}

void IndexedListModel::popEmpty()
{
	for (int i = innerList.count() - 1; i > -1; --i)
	{
		if (innerList.at(i).index < -1)
		{
			beginRemoveRows(createIndex(i, 0), i, i);
			innerList.removeAt(i);
			endRemoveRows();
			return;
		}
	}
}

void IndexedListModel::pushJoin()
{
	beginInsertRows(createIndex(innerList.count() - 1, 0), innerList.count() - 1, innerList.count());
	innerList << indexedString(tr("Join"), -3);
	endInsertRows();
}




indexedString& IndexedListModel::data(int index)
{
	return innerList[index];
}

QList<int> IndexedListModel::getSelectedIndexes()
{
	QList<int> temp;
	for (int i = 0; i < innerList.count(); ++i)
	{
		if (innerList.at(i).isSelected)
		{
			temp << innerList.at(i).index;
		}
	}
	return temp;
}

QList<int> IndexedListModel::getSelected()
{
	QList<int> temp;
	for (int i = 0; i < innerList.count(); ++i)
	{
		if (innerList.at(i).isSelected)
		{
			temp << i;
		}
	}
	return temp;
}

void IndexedListModel::selectIndexes(const QList<int>& indexes)
{
	for (int i = 0; i < indexes.count(); ++i)
	{
		if (indexes[i] >= 0 && indexes[i] < innerList.count())
		{
			innerList[indexes[i]].isSelected = true;
		}
	}
}

void IndexedListModel::selectItem(const QModelIndex& ind)
{
	if (ind.isValid())
	{
		innerList[ind.row()].isSelected = !innerList.at(ind.row()).isSelected;
		dataChanged(ind, ind);
	}
}
