#pragma once
#include <QWidget>
#include <QBoxLayout>
#include "widgets/utils/MegaIconButton.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <qprogressbar.h>
#include "externalCommunication/tohttp.h"

class SearchDatabaseSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLineEdit* httpDownloadUrl;
	QLineEdit* httpUploadUrl;
	MegaIconButton* downloadNow;
	QProgressBar* downloadProgress;
	toHttp downloadcenter;
	QLineEdit* databasePath;
	MegaIconButton* fromFileLoad;
	QTimer* timeoutTimer;
public:
	SearchDatabaseSettings(QWidget* parent);
	void extractAndSave();
	void retranslate();
protected slots:
	void downloadDatabase();
	void loadDatabaseFromFile();
	void downloadTimeout();
	void downloadProcess(int);
};