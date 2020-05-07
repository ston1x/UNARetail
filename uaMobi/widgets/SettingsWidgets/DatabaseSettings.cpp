#include "DatabaseSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include <QMessageBox>


DatabaseSettings::DatabaseSettings(Modes mode, QWidget* parent)
	:QWidget(parent), mainLayout(new QVBoxLayout(this)), 
	clearScanned(new MegaIconButton(this)),
	wipeDatabase(new MegaIconButton(this)), clearLogs(new MegaIconButton(this)),
	searchOnScan(new MegaIconButton(this)), currentMode(mode)
{
	mainLayout->addWidget(clearScanned);
	mainLayout->addWidget(wipeDatabase);
	mainLayout->addWidget(clearLogs);
	mainLayout->addWidget(searchOnScan);

	clearScanned->setIcon(QIcon(":/res/deleteData.png"));
	wipeDatabase->setIcon(QIcon(":/res/data.png"));
	clearLogs->setIcon(QIcon(":/res/minus.png"));
	searchOnScan->setIcon(QIcon(":/res/find3 (2).png"));

	clearScanned->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	wipeDatabase->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	clearLogs->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	searchOnScan->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);

	clearScanned->setText(tr("Clear scanned"));
	wipeDatabase->setText(tr("Wipe database"));
	clearLogs->setText(tr("Clear logs"));
	searchOnScan->setText(tr("Search on scan"));
	
	searchOnScan->setCheckable(true);
	searchOnScan->setChecked(AppSettings->autoSearch);
#ifdef QT_VERSION5X
	QObject::connect(clearScanned, &MegaIconButton::clicked, this, &DatabaseSettings::clearScannedConfirmed);
	QObject::connect(wipeDatabase, &MegaIconButton::clicked, this, &DatabaseSettings::wipeDatabaseConfirmed);
	QObject::connect(clearLogs, &MegaIconButton::clicked, this, &DatabaseSettings::clearLogsConfirmed);
#else
	QObject::connect(clearScanned, SIGNAL(clicked()), this, SLOT(clearScannedConfirmed()));
	QObject::connect(wipeDatabase, SIGNAL(clicked()), this, SLOT(wipeDatabaseConfirmed()));
	QObject::connect(clearLogs, SIGNAL(clicked()), this, SLOT(clearLogsConfirmed()));
#endif
	
}

void DatabaseSettings::extractAndSave()
{
	AppSettings->autoSearch = searchOnScan->isChecked();
}

void DatabaseSettings::clearScannedConfirmed()
{
	int result = QMessageBox::question(this, tr("Deleting scanned"), tr("Do you wish deleting scanned?"), QMessageBox::Ok, QMessageBox::Abort);
	if (result == QMessageBox::Ok)
	{
		AppData->dropTableOf(TableNames::Scanned, currentMode);
		AppData->createTableOf(TableNames::Scanned, currentMode);
	}
}

void DatabaseSettings::wipeDatabaseConfirmed()
{
	int result = QMessageBox::question(this, tr("Deleting scanned"), tr("Do you wish deleting scanned?"), QMessageBox::Ok, QMessageBox::Abort);
	if (result == QMessageBox::Ok)
	{
		AppData->wipeAll();
	}
}

void DatabaseSettings::clearLogsConfirmed()
{
	AppBackup->wipeOld();
}
