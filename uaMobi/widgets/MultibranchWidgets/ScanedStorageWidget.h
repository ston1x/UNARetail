#pragma once
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/QLabel>
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "Datacore/DataEntities.h"
#include "dataproviders/sqldataprovider.h"
#include "dataproviders/DataAsyncLoader.h"
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
	QWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* countersLayout;
	QLabel* totalQuantity;
	QLabel* uniqueBarcodes;
	
	DataEntityListModel* model;
	QListView* barcodeView;
	
	MegaIconButton* backButton;

	QThread* dataloadThread;
	DataAsyncLoader* asyncloader;
	Modes currentMode;
public:
	ScanedStorageWidget(Modes mode,  QWidget* parent);
	
	void addEntity(Entity);
	virtual void show() override;	//	auto-refresh on show
	virtual bool back() override;
	void refresh();
public slots:
	void drop();
	void dataLoaded(EntityList);
signals:
	void startDataLoad(Modes mode);
};
