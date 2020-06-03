#pragma once
#include <QWidget>
#include "Datacore/ShortBarcodeEntity.h"
#include <qabstractitemmodel.h>
#include <qmetatype.h>




class QTextEdit;
class QSpinBox;
class MegaIconButton;
class QLabel;
class QFormLayout;
class QLineEdit;
class QComboBox;
class QListView;
class QHBoxLayout;
class QVBoxLayout;
struct indexedString
{
	QString string;
	int index;
	bool isSelected;
	indexedString(QString str = QString(), int ind = 0) : string(str), index(ind), isSelected(false) {}
	inline indexedString& operator=(const indexedString& other) { 
		string = other.string; index = other.index; isSelected = other.isSelected; return *this;
	};
	inline QString serialize() const { return string + "," + QString::number(index) + "\n"; }
};

Q_DECLARE_METATYPE(indexedString);

class IndexedListModel : public QAbstractListModel
{
	Q_OBJECT
protected:
	// real data storage
	QVector<indexedString> innerList;
public:
	enum DataRoles {SelectionRole = Qt::UserRole + 1};
	IndexedListModel(QObject* parent = Q_NULLPTR) : QAbstractListModel(parent) {}
	// Inherited from QAbstractListModel
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
	virtual bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
	virtual Qt::DropActions supportedDragActions() const override;
	virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
	virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	virtual bool setItemData(const QModelIndex& index, const QMap<int, QVariant>& roles) override;
	virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
	virtual bool canDropMimeData(const QMimeData* data,
		Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
	virtual QStringList mimeTypes() const override;
	virtual Qt::DropActions supportedDropActions() const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	// dataoperation methods
	//post-constructor
	void insertData(const QStringList& data);
	void insertData(const QVector<indexedString>& v);
	void pushEmpty();
	void popEmpty();
	void pushJoin();
	indexedString& data(int index);
	QList<int> getSelectedIndexes();
	QList<int> getSelected();
	void selectIndexes(const QList<int>& indexes);
public slots:
	void selectItem(const QModelIndex&);
	
};


class DatabaseLoadSettings : public QWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	IndexedListModel* model;
	QLabel* databasePathInfo;
	QTextEdit* databasePath;
	QLabel* separatorCodeInfo;
	QSpinBox* separatorCode;
	QListView* fieldSelector;
	QHBoxLayout * controlLayout;
	MegaIconButton* deleteButton;
	MegaIconButton* addButton;
	MegaIconButton* joinButton;
	QHBoxLayout* lowerLayout;
	MegaIconButton* dropDatabaseButton;
	MegaIconButton* fromFileLoad;


	QVector<int> extractInsertionIndexes();

	QVector<indexedString> _extractFields();
	QList<int> _extractPoints();

	void _addToBarcode(int index, ShortBarcode & shb, const QString& parsedColumn);

public:
	DatabaseLoadSettings(QWidget* parent);
	void extractAndSave();
	void retranslate();
protected slots:
	void loadDatabaseFromFile();
	void addEmpty();
	void deleteEmpty();
	void addJoin();
	void dropDB();
};