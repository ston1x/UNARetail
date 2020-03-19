#include "ScanedStorageWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include <QtWidgets/QScroller>
#include "widgets/utils/ZebraListItemDelegate.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

ScanedStorageWidget::ScanedStorageWidget(Modes mode, QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	countersLayout(new QHBoxLayout(innerWidget)), totalQuantity(new QLabel(innerWidget)),
	uniqueBarcodes(new QLabel(innerWidget)),
	model(new DataEntityListModel(this)),
	barcodeView(new QListView(innerWidget)),
	backButton(new MegaIconButton(innerWidget)),
	dataloadThread(new QThread(this)), asyncloader(new DataAsyncLoader(this)),
	currentMode(mode)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addLayout(countersLayout);
	innerLayout->addWidget(barcodeView);
	innerLayout->addWidget(backButton);

	mainLayout->setSpacing(0);				//	Removed spacing for both layouts
	mainLayout->setContentsMargins(0, 0, 0, 0);

	countersLayout->addWidget(totalQuantity);
	countersLayout->addWidget(uniqueBarcodes);

	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);

	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("branch_root_back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	barcodeView->setVerticalScrollMode(QListView::ScrollPerPixel);
	barcodeView->setItemDelegate(new ZebraItemDelegate(barcodeView));
	barcodeView->setModel(model);
	barcodeView->setFont(AppFonts->makeFont(2.0));	

	totalQuantity->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	uniqueBarcodes->setStyleSheet(COUNTERS_LABEL_STYLESHEET);

	QObject::connect(asyncloader, &DataAsyncLoader::resultReady, this, &ScanedStorageWidget::dataLoaded);
	QObject::connect(this, &ScanedStorageWidget::startDataLoad, asyncloader, 
		&DataAsyncLoader::loadAllEntitiesForMode);
	asyncloader->moveToThread(dataloadThread);
	refresh();

    QObject::connect(backButton, &QPushButton::clicked, this, &ScanedStorageWidget::backRequired);
}

ScanedStorageWidget::~ScanedStorageWidget()
{
	if (!dataloadThread->isFinished())
		dataloadThread->exit(0);
	asyncloader->moveToThread(thread());
    asyncloader->deleteLater();
}

void ScanedStorageWidget::refresh()
{
	dataloadThread->start();
	emit startDataLoad(currentMode);
}

void ScanedStorageWidget::addEntity(Entity e)
{
	model->appendDataEntity(e);
}

void ScanedStorageWidget::show() // overriden - autorefresh added
{
	totalQuantity->setText(tr("storage_total_quantity\n") +
		QString::number(
			AppData->countAllIn(
				currentMode, TableNames::Scanned)
			+
			AppData->countAllIn(
				currentMode, TableNames::Uploaded)));
	uniqueBarcodes->setText(tr("storage_uniques\n") +
		QString::number(
			AppData->countUniqueIn(
				currentMode, TableNames::Scanned)
			+
			AppData->countUniqueIn(
				currentMode, TableNames::Uploaded)));
	inframedWidget::show();
}

bool ScanedStorageWidget::back()
{
	return true;
}

void ScanedStorageWidget::dataLoaded(EntityList elist)
{
	model->insertData(elist);
	dataloadThread->exit();
}

void ScanedStorageWidget::drop()
{
	model->reset();
	AppData->dropTableOf(TableNames::Scanned, currentMode);
	AppData->dropTableOf(TableNames::Uploaded, currentMode);
	AppData->createTableOf(TableNames::Scanned, currentMode);
	AppData->createTableOf(TableNames::Uploaded, currentMode);
}

