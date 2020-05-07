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
#include <qlistwidget.h>
#include "externalCommunication/tohttp.h"
#include "widgets/parents/abstractNodeInterface.h"

class SearchDatabaseSettings : public QWidget,  abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* innerLayout;
	inframedWidget* innerWidget;
	QFormLayout* mainLayout;
	QLineEdit* httpDownloadUrl;
	QHBoxLayout* placeLayout;
	QLabel* placeInfo;
	MegaIconButton* selectPlaceButton;
	MegaIconButton* deletePlaceButton;
	MegaIconButton* downloadNow;
	QProgressBar* downloadProgress;
	toHttp downloadcenter;
	QTimer* timeoutTimer;

	inframedWidget* placeSelectionWidget;
	QVBoxLayout* placeSelectionLayout;
	QListWidget* placeSelectionList;
	MegaIconButton* backButton;

	QStringList placeCodes;
	QStringList placeNames;
	QString currentPlace;
	QString currentPlaceName;

	QString _getCurrentPlace();
public:
	SearchDatabaseSettings(QWidget* parent);
	void extractAndSave();
	void retranslate();
protected slots:
	void downloadDatabase();
	void downloadTimeout();
	void downloadProcess(int);
	void getPlaceList();
	void placeListReceived(QStringList names, QStringList codes);
	void placeListTimeout();
	void placeSelected(QListWidgetItem*);
	void hideCurrent();
	void productListReceived(QLinkedList<ShortBarcode>& products);
	void downloadError();
	void deletePlace();
};