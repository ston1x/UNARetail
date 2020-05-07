#include "ScanedStorageWidget.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef Q_OS_ANDROID
#include <QtWidgets/QScroller>
#endif
#include "widgets/utils/ZebraListItemDelegate.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include <qmessagebox.h>


ScanedStorageWidget::ScanedStorageWidget(Modes mode, QWidget* parent)
	: inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	countersLayout(new QHBoxLayout(innerWidget)), totalQuantity(new QLabel(innerWidget)),
	uniqueBarcodes(new QLabel(innerWidget)),
	model(new DataEntityListModel(this)),
	barcodeView(new QListView(innerWidget)),
	redactingLayout(new QHBoxLayout(innerWidget)),
	editButton(new MegaIconButton(innerWidget)),
	deleteButton(new MegaIconButton(innerWidget)),
	backButton(new MegaIconButton(innerWidget)),
    dataloadThread(new QThread(this)), asyncloader(new DataAsyncLoader()),
	redacting(new BarcodeRedactingWidget(mode, this)),
	currentMode(mode)
{
	main = this;
	current = untouchable = innerWidget;
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addLayout(countersLayout);
	innerLayout->addWidget(barcodeView);
	innerLayout->addLayout(redactingLayout);
	redactingLayout->addWidget(editButton);
	redactingLayout->addWidget(deleteButton);
	innerLayout->addWidget(backButton);

	mainLayout->setSpacing(0);				//	Removed spacing for both layouts
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(redacting);
	redacting->hide();

	countersLayout->addWidget(totalQuantity);
	countersLayout->addWidget(uniqueBarcodes);

	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);

	editButton->setIcon(QIcon(":/res/editable.png"));
	deleteButton->setIcon(QIcon(":/res/deleteData.png"));
	editButton->setText(tr("Edit"));
	deleteButton->setText(tr("Delete"));
	editButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	deleteButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);


	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("branch_root_back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	barcodeView->setVerticalScrollMode(QListView::ScrollPerPixel);
	barcodeView->setItemDelegate(new ZebraItemDelegate(barcodeView));
	barcodeView->setModel(model);
	barcodeView->setFont(AppFonts->makeFont(2.0));	
	barcodeView->setSelectionMode(QListView::SingleSelection);

	totalQuantity->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	uniqueBarcodes->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
#ifdef Q_OS_ANDROID
	QScroller::grabGesture(barcodeView, QScroller::LeftMouseButtonGesture);
#endif
#ifdef QT_VERSION5X
	QObject::connect(asyncloader, &DataAsyncLoader::resultReady, this, &ScanedStorageWidget::dataLoaded);
	QObject::connect(this, &ScanedStorageWidget::startDataLoad, asyncloader, 
		&DataAsyncLoader::loadAllEntitiesForMode);
	asyncloader->moveToThread(dataloadThread);
	refresh();
    QObject::connect(backButton, &QPushButton::clicked, this, &ScanedStorageWidget::backRequired);
	QObject::connect(editButton, &MegaIconButton::clicked, this, &ScanedStorageWidget::editCurrent);
	QObject::connect(deleteButton, &MegaIconButton::clicked, this, &ScanedStorageWidget::deleteCurrent);
	QObject::connect(redacting, &BarcodeRedactingWidget::barcodeCommited, this, &ScanedStorageWidget::editingCompleted);
	QObject::connect(redacting, &BarcodeRedactingWidget::backRequired, this, &ScanedStorageWidget::hideCurrent);
#else
	QObject::connect(asyncloader, SIGNAL(resultReady(EntityList)), this, SLOT(dataLoaded(EntityList)));
    QObject::connect(this, SIGNAL(startDataLoad(Modes)), asyncloader,
        SLOT(loadAllEntitiesForMode(Modes)));
	asyncloader->moveToThread(dataloadThread);
	refresh();
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(editButton, SIGNAL(clicked()), this, SLOT(editCurrent()));
	QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrent()));
	QObject::connect(redacting, SIGNAL(barcodeCommited(Entity, Entity)), this, SLOT(editingCompleted(Entity, Entity)));
	QObject::connect(redacting, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
}

ScanedStorageWidget::~ScanedStorageWidget()
{
	if (!dataloadThread->isFinished())
        dataloadThread->exit(0);
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

void ScanedStorageWidget::editCurrent()
{
	if (barcodeView->currentIndex().isValid())
	{
		if (redacting->setBarcode(
			barcodeView->currentIndex().data(
				DataEntityListModel::DirectAccessRole).value<Entity>()))
			_hideAny(redacting);
	}
}

void ScanedStorageWidget::deleteCurrent()
{
	if (barcodeView->currentIndex().isValid())
	{
		Entity e = 
			barcodeView->currentIndex().data(
				DataEntityListModel::DirectAccessRole).value<Entity>();
		if (e != Q_NULLPTR)
		{
			if (e->myType() == int(barcodeUtil::barcodetypes::separatorNotBC))
				return;
			int answer = QMessageBox::question(this,
				tr("Deleting"),
				tr("Do you wish to delete barcode ") + e->getName() + "?",
				QMessageBox::Ok, QMessageBox::Cancel
			);
			if (answer == QMessageBox::Ok)
			{
				AppData->deleteEntity(currentMode, e, TableNames::Scanned);
				model->removeDataEntity(e);
				AppBackup->pushOperation(OpType::DELE, int(currentMode), e);
			}
		}
	}
}

void ScanedStorageWidget::editingCompleted(Entity oldOne, Entity newOne)
{
	AppData->replaceEntityIn(currentMode, oldOne, newOne, TableNames::Scanned);
	AppBackup->pushOperation(OpType::EDIT, int(currentMode), newOne);
	model->replaceDataEntity(newOne);
}

void ScanedStorageWidget::hideCurrent()
{
	_hideCurrent(untouchable);
}

void ScanedStorageWidget::drop()
{
	model->reset();
	AppData->dropTableOf(TableNames::Scanned, currentMode);
	AppData->dropTableOf(TableNames::Uploaded, currentMode);
	AppData->createTableOf(TableNames::Scanned, currentMode);
	AppData->createTableOf(TableNames::Uploaded, currentMode);
	AppBackup->pushOperation(OpType::WIPE, int(currentMode));
}

