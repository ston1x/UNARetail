#pragma once
#include <qlistview.h>
#include <qboxlayout.h>
#include <QLabel>
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "Datacore/DataEntities.h"
#include "dataproviders/sqldataprovider.h"
#include "dataproviders/DataAsyncLoader.h"
#include "BarcodeRedactingWidget.h"
/*
	This widget is made to provide access to barcode database with possibility to edit entries.
	Each barcode is shown like a string with all info barcode is holding.

	Update:
		Removed editing
		Cleared renundant headers
	Update:
		minor changes in overloading-> fixed bug with show()
	Update:
		major changes into barcodelist holder. Now this widget is supporting touch scroll
		and uses a delegate for better separation of elements. Also renundant selection
		was removed and all entries are sorted by default using addDate.
	Update:
		minor changes: now dateFormat is the same with database
	Update:
		Now additional vector link is storing - with uploaded barcodes
		now separator is used to separate sent barcodes and unsent ones
		minor changes into refresh - now it also shows sent barcodes
*/
class ScanedStorageWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* countersLayout;
	QLabel* totalQuantity;
	QLabel* uniqueBarcodes;
	
	DataEntityListModel* model;
	QListView* barcodeView;
	
	QHBoxLayout* redactingLayout;
	MegaIconButton* editButton;
	MegaIconButton* deleteButton;
	MegaIconButton* backButton;

	QThread* dataloadThread;
	DataAsyncLoader* asyncloader;

	BarcodeRedactingWidget* redacting;

	Modes currentMode;
public:
	ScanedStorageWidget(Modes mode,  QWidget* parent);
    virtual ~ScanedStorageWidget();
	void addEntity(Entity);
	virtual void show() override;	//	auto-refresh on show
	virtual bool back() override;
	void refresh();
protected slots:
	void drop();
	void dataLoaded(EntityList);
	void editCurrent();
	void deleteCurrent();
	void editingCompleted(Entity, Entity);
	void hideCurrent();
signals:
	void startDataLoad(Modes mode);
};
