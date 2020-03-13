#pragma once
#include <QWidget>
#include <QBoxLayout>
#include "widgets/utils/MegaIconButton.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>
#include "dataproviders/sqldataprovider.h"



class DatabaseSettings : public QWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	MegaIconButton* clearScanned;
	MegaIconButton* wipeDatabase;
	MegaIconButton* clearLogs;
	MegaIconButton* searchOnScan;

	Modes currentMode;
public:
	DatabaseSettings(Modes mode, QWidget* parent);
	void extractAndSave();
protected slots:
	void clearScannedConfirmed();
	void wipeDatabaseConfirmed();
	void clearLogsConfirmed();
};