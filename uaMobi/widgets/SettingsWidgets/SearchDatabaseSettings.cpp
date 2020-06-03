#include "SearchDatabaseSettings.h"
#include <QTimer>
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "dataproviders/sqldataprovider.h"
#include <QMessageBox>
#ifdef Q_OS_ANDROID
#include <qscroller.h>
#endif

QString SearchDatabaseSettings::_getCurrentPlace()
{
	if (currentPlace.isEmpty())
		return "";
	else
		return "&place=" + currentPlace;
}

SearchDatabaseSettings::SearchDatabaseSettings(QWidget* parent)
	: QWidget(parent), abstractNode(),
	innerLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)),
	mainLayout(new QFormLayout(innerWidget)),
	storedBarcodes(new QLabel(innerWidget)),
	httpDownloadUrl(new QLineEdit(innerWidget)),
	placeLayout(new QHBoxLayout(innerWidget)),
	placeInfo(new QLabel(innerWidget)),
	selectPlaceButton(new MegaIconButton(innerWidget)),
	deletePlaceButton(new MegaIconButton(innerWidget)),
	downloadNow(new MegaIconButton(innerWidget)),
	downloadProgress(new QProgressBar(innerWidget)),
	downloadcenter(Modes::Search),
	timeoutTimer(new QTimer(this)),

	placeSelectionWidget(new inframedWidget(this)),
	placeSelectionLayout(new QVBoxLayout(placeSelectionWidget)),
	placeSelectionList(new QListWidget(placeSelectionWidget)),
	backButton(new MegaIconButton(placeSelectionWidget)),
	placeCodes(), placeNames(),
	currentPlace(AppSettings->placeAsCode),
	currentPlaceName(AppSettings->placeAsItem)
{
	main = innerWidget;
	current = innerWidget;
	untouchable = innerWidget;
	innerLayout->addWidget(innerWidget);
	innerWidget->setLayout(mainLayout);
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	mainLayout->addRow(tr("size of the base"), storedBarcodes);
	mainLayout->addRow(tr("Remote database url"), httpDownloadUrl);
	mainLayout->addWidget(downloadProgress);
	mainLayout->addItem(placeLayout);
	
	placeLayout->addWidget(placeInfo);
	placeLayout->addWidget(selectPlaceButton);
	placeLayout->addWidget(deletePlaceButton);
	mainLayout->addWidget(downloadNow);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);

	placeLayout->setContentsMargins(0, 0, 0, 0);
	placeLayout->setSpacing(0);
	mainLayout->setLabelAlignment(Qt::AlignCenter);
	mainLayout->setFormAlignment(Qt::AlignCenter);
	mainLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);

	innerLayout->addWidget(placeSelectionWidget);
	placeSelectionWidget->setLayout(placeSelectionLayout);
	placeSelectionLayout->addWidget(placeSelectionList);
	placeSelectionLayout->addWidget(backButton);
	placeSelectionLayout->setContentsMargins(0, 0, 0, 0);
	placeSelectionLayout->setSpacing(0);

	downloadNow->setText(tr("Download"));
	downloadNow->setIcon(QIcon(":/res/netDownload.png"));
	downloadNow->setStyleSheet(DOWNLOAD_BUTTONS_STYLESHEET);

	selectPlaceButton->setText(tr("Places"));
	selectPlaceButton->setIcon(QIcon(":/res/rec2.png"));
	deletePlaceButton->setText(tr("del"));
	deletePlaceButton->setIcon(QIcon(":/res/data2.png"));
	httpDownloadUrl->setText(AppSettings->httpIn.toString());
	downloadcenter.setAddress(AppSettings->httpIn.toString());
	if (!currentPlace.isEmpty())
	{
		placeInfo->setText(AppSettings->placeAsItem);
	}
	else
	{
		placeInfo->setText(tr("no place"));
	}
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
#ifdef Q_OS_ANDROID
	QScroller::grabGesture(placeSelectionList, QScroller::LeftMouseButtonGesture);
	placeSelectionList->setFont(AppFonts->makeFont(2.0));
#endif
	placeSelectionList->setHorizontalScrollMode(QListView::ScrollPerPixel);
	placeSelectionList->setVerticalScrollMode(QListView::ScrollPerPixel);
	placeSelectionList->setWordWrap(true);
	timeoutTimer->setInterval(40000);
	timeoutTimer->setSingleShot(true);
	placeSelectionWidget->hide();
	downloadProgress->setTextVisible(true);
	downloadProgress->setAlignment(Qt::AlignCenter);
#ifdef QT_VERSION5X
	QObject::connect(downloadNow, &QPushButton::clicked, this, &SearchDatabaseSettings::downloadDatabase);
	QObject::connect(&downloadcenter, &toHttp::progressLeap, this, &SearchDatabaseSettings::downloadProcess);
	QObject::connect(timeoutTimer, &QTimer::timeout, this, &SearchDatabaseSettings::downloadTimeout);
	QObject::connect(deletePlaceButton, &MegaIconButton::clicked, this, &SearchDatabaseSettings::deletePlace);

	QObject::connect(selectPlaceButton, &MegaIconButton::clicked, this, &SearchDatabaseSettings::getPlaceList);
	QObject::connect(&downloadcenter, &toHttp::placelistReceived, this, &SearchDatabaseSettings::placeListReceived);
	QObject::connect(placeSelectionList, &QListWidget::itemDoubleClicked, this, &SearchDatabaseSettings::placeSelected);
	QObject::connect(&downloadcenter, &toHttp::downloadStateChanged, this, &SearchDatabaseSettings::downloadStateChanged);
	QObject::connect(&downloadcenter, &toHttp::errorReceived, this, &SearchDatabaseSettings::downloadTimeout);
#else
	QObject::connect(httpDownloadUrl, SIGNAL(returnPressed()), httpUploadUrl,
		SLOT(setFocus()));
	QObject::connect(downloadNow, SIGNAL(clicked()), this, SLOT(downloadDatabase()));
	QObject::connect(&downloadcenter, SIGNAL(progressLeap(int)), this, SLOT(downloadProcess(int)));
	QObject::connect(timeoutTimer,SIGNAL(timeout()), this, SLOT(downloadTimeout()));
#endif
	refreshStoredCounter();
}

void SearchDatabaseSettings::extractAndSave()
{
	AppSettings->httpIn = QUrl::fromUserInput(httpDownloadUrl->text());
}

void SearchDatabaseSettings::retranslate()
{
	qobject_cast<QLabel*>(mainLayout->labelForField(httpDownloadUrl))->setText(tr("Remote database url"));
	
	downloadNow->setText(tr("Download"));
	
}

void SearchDatabaseSettings::downloadDatabase()
{
	if (downloadcenter.isUsed())
		return;
	downloadProgress->setValue(1);
	downloadNow->setDisabled(true);
	timeoutTimer->start();
	downloadcenter.setAddress(httpDownloadUrl->text());
	downloadcenter.getProductList(_getCurrentPlace());
}



void SearchDatabaseSettings::downloadTimeout()
{
	downloadcenter.dropAwaiting();
	downloadNow->setDisabled(false);
	QMessageBox::critical(this, tr("error"), tr("TimeoutError"));
}

void SearchDatabaseSettings::downloadProcess(int proc)
{
	downloadProgress->setValue(proc);
	timeoutTimer->stop();
	if (proc == 100)
	{
		downloadNow->setDisabled(false);
	}
}

void SearchDatabaseSettings::getPlaceList()
{
	if (!downloadcenter.isUsed())
	{
		downloadcenter.setAddress(httpDownloadUrl->text());
		timeoutTimer->start();
		if (!downloadcenter.getPlacelist())
			QMessageBox::critical(this, tr("error"), tr("Error receiving place list"));
	}
}

void SearchDatabaseSettings::placeListReceived(QStringList names, QStringList codes)
{
	if (names.count() != codes.count())
	{
		QMessageBox::critical(this, tr("error"), tr("data error: codes count do not correspond to names count!"));
	}
	placeCodes = codes;
	placeNames = names;
	_hideAny(placeSelectionWidget);
	placeSelectionList->clear();
	placeSelectionList->addItems(names);
}

void SearchDatabaseSettings::placeListTimeout()
{
	QMessageBox::critical(this, tr("error"), tr("TimeoutError"));
}

void SearchDatabaseSettings::placeSelected(QListWidgetItem* item)
{
	int index = placeSelectionList->row(item);
	if (index >= 0 && index < placeNames.count())
	{
		_hideCurrent(untouchable);
		currentPlace = placeCodes.at( index);
		placeInfo->setText(placeNames.at(index));
		AppSettings->placeAsCode = placeCodes.at(index);
		AppSettings->placeAsItem = placeNames.at(index);
	}
}

void SearchDatabaseSettings::hideCurrent()
{
	_hideCurrent(untouchable);
}

void SearchDatabaseSettings::downloadStateChanged(QString state)
{
	if (state.isEmpty())
	{
		int downloadedQ = downloadcenter.count();
		downloadcenter.clear();
		QMessageBox::information(this, tr("done"), tr("Product list received succesfully") + " " + QString::number(downloadedQ));
		refreshStoredCounter();
	}
	else
	{
		downloadProgress->setFormat(state);
	}
}


void SearchDatabaseSettings::downloadError()
{
	QMessageBox::critical(this, tr("connection error"), tr("connection error - download stopped"));
	downloadcenter.dropAwaiting();
}

void SearchDatabaseSettings::deletePlace()
{
	AppSettings->placeAsCode.clear();
	AppSettings->placeAsItem.clear();
	currentPlace.clear();
	currentPlaceName.clear();
	placeInfo->setText(tr("no place"));
}

void SearchDatabaseSettings::refreshStoredCounter()
{
	storedBarcodes->setText(QString::number(AppData->countDownloaded()));
}
