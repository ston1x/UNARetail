#include "AbstractScaningWidget.h"
#ifdef QT_VERSION5X
#include <QScroller>
#endif
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/utils/ZebraListItemDelegate.h"
#include "widgets/utils/GlobalAppSettings.h"
AbstractScaningWidget::AbstractScaningWidget(Modes mode, QWidget* parent)
	: inframedWidget(parent), abstractDynamicNode(new inframedWidget(this), new QVBoxLayout(this)),
	innerLayout(new QVBoxLayout(untouchable)), counterLayout(new QHBoxLayout(untouchable)),
	controlLayout(new QVBoxLayout(untouchable)),
	buttonLayout(new QHBoxLayout(untouchable)),
	additionalInputLayout(new QHBoxLayout(untouchable)),
	modeName(new QLabel(untouchable)),
	barcodeInfo(new QTextEdit(untouchable)),
	barcodeInput(new QLineEdit(untouchable)),
	backButton(new MegaIconButton(untouchable)),
	keyboardButton(new MegaIconButton(untouchable)),
#ifdef CAMERA_SUPPORT
	cameraButton(new MegaIconButton(untouchable)),
#endif
	barcodeModel(),
	historyView(),
	currentMode(mode)
{
	

	untouchable->setLayout(innerLayout);
	innerLayout->addWidget(modeName);
	innerLayout->addWidget(barcodeInput);
	innerLayout->addWidget(barcodeInfo);
	innerLayout->addLayout(counterLayout);
	innerLayout->addLayout(controlLayout);

	if (AppSettings->showHistory)
	{
		barcodeModel = new DataEntityListModel(this);
		historyView = new QListView(this);
		historyView->setItemDelegate(new ShortZebraItemDelegate(historyView));
		historyView->setModel(barcodeModel);
		historyView->setFont(AppFonts->makeFont(0.8));
		historyView->setSelectionMode(QListView::NoSelection);
		historyView->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerItem);
		innerLayout->addWidget(historyView);
#ifdef Q_OS_ANDROID
		QScroller::grabGesture(historyView, QScroller::LeftMouseButtonGesture);
#endif
	}
	else
	{
		innerLayout->addStretch(0);
	}

	innerLayout->addLayout(additionalInputLayout);
	innerLayout->addLayout(buttonLayout);
	
	
	buttonLayout->addWidget(backButton);
	additionalInputLayout->addWidget(keyboardButton);
#ifdef CAMERA_SUPPORT
	additionalInputLayout->addWidget(cameraButton);
	cameraButton->setIcon(QIcon(":/res/camera.png"));
	QObject::connect(cameraButton, &QPushButton::clicked, this, &AbstractScaningWidget::cameraRequired);
#endif

	// Layouts are reversal so that new elements were pushed to left
	buttonLayout->setDirection(QBoxLayout::RightToLeft);

	// No any spacings or margins
	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	counterLayout->setSpacing(0);
	counterLayout->setContentsMargins(0, 0, 0, 0);
	controlLayout->setSpacing(0);
	controlLayout->setContentsMargins(0, 0, 0, 0);
	// Set up view
	backButton->setIcon(QIcon(":/res/back.png"));
	
	keyboardButton->setIcon(QIcon(":/res/key.png"));

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

//	barcodeInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	barcodeInfo->setFixedHeight(calculateAdaptiveButtonHeight());
	barcodeInfo->setFont(AppFonts->makeFont(3.0));
	barcodeInfo->setWordWrapMode(QTextOption::WordWrap);
	barcodeInfo->setAlignment(Qt::AlignCenter);
	QPalette p;
	p.setColor(QPalette::Base, qApp->palette().background().color());
	barcodeInfo->setPalette(p);
	//barcodeInfo->setTextBackgroundColor(qApp->palette().background().color());
	modeName->setFont(AppFonts->makeFont(2.0));
	modeName->setAlignment(Qt::AlignCenter);
	modeName->setText(transformModeToString(currentMode));

	barcodeInput->setMinimumHeight(calculateAdaptiveButtonHeight());
	barcodeInput->setFont(AppFonts->makeFont(3.0));
	barcodeInput->setAlignment(Qt::AlignCenter);

	if (!AppSettings->additionalControlElements)
	{
#ifdef CAMERA_SUPPORT
		cameraButton->hide();
#endif
		keyboardButton->hide();
	}
	if (!AppSettings->navigationElements)
	{
		backButton->hide();
	}

	

	BarcodeObs->activate();
#ifdef QT_VERSION5X
	QObject::connect(backButton, &QPushButton::clicked, this, &AbstractScaningWidget::backRequired);
	QObject::connect(keyboardButton, &MegaIconButton::clicked, this, &AbstractScaningWidget::keyboardRequired);
	
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(BarcodeObs, &BarcodeObserver::scanButtonPress, this, &AbstractScaningWidget::handleScanButton);
	QObject::connect(BarcodeObs, &BarcodeObserver::backButtonPress, this, &AbstractScaningWidget::backRequired);
	QObject::connect(barcodeInput, &QLineEdit::returnPressed, this, &AbstractScaningWidget::manualConfirmed);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(keyboardButton, SIGNAL(clicked()), this, SLOT(keyboardRequired()));

	QObject::connect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(barcodeConfirmed(QString)));
	QObject::connect(BarcodeObs, SIGNAL(scanButtonPress()), this, SLOT(handleScanButton()));
	QObject::connect(BarcodeObs, SIGNAL(backButtonPress()), this, SIGNAL(backRequired()));
	QObject::connect(barcodeInput, SIGNAL(returnPressed()), this, SLOT(manualConfirmed()));
#endif
}

void AbstractScaningWidget::show()
{
	if (AppSettings->additionalControlElements)
	{
		keyboardButton->show();
#ifdef CAMERA_SUPPORT
		cameraButton->show();
#endif
	}
	else
	{
		keyboardButton->hide();
#ifdef CAMERA_SUPPORT
		cameraButton->hide();
#endif
	}
	if (AppSettings->navigationElements)
	{
		backButton->show();
	}
	else
	{
		backButton->hide();
	}
	inframedWidget::show();
	BarcodeObs->activate();
}

void AbstractScaningWidget::hide()
{
	BarcodeObs->deactivate();
	inframedWidget::hide();
}

void AbstractScaningWidget::barcodeConfirmed(QString barcode)
{
	_emplaceBarcode(barcode);
	_clearControls();
}
#ifdef CAMERA_SUPPORT
void AbstractScaningWidget::cameraRequired()
//	This slot creates new camera widget because only one camera widget can exist
{
	_hideAndDeleteCurrent(new ScaningCameraWidget(this));
	QObject::connect(_upCO<ScaningCameraWidget>(), &ScaningCameraWidget::backRequired, this, &AbstractScaningWidget::hideCurrent);
	QObject::connect(_upCO<ScaningCameraWidget>(), &ScaningCameraWidget::hasBarcode, this, &AbstractScaningWidget::handleCameraBarcode);
}
#endif
void AbstractScaningWidget::keyboardRequired()
//	This slot creates new keyboard
{
	_hideAndDeleteCurrent(new VirtualBarcodeKeyboard(this));
#ifdef QT_VERSION5X
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::backRequired, this, &AbstractScaningWidget::hideCurrent);
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::barcodeConfirmed, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::valueConfirmed, this, &AbstractScaningWidget::handleValueFromKeyboard);
#else
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), SIGNAL(barcodeConfirmed(QString)), this, SLOT(barcodeConfirmed()));
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), SIGNAL(valueConfirmed(QString)), this, SLOT(handleValueFromKeyboard()));
#endif
}

void AbstractScaningWidget::hideCurrent()
{
	_hideAndDeleteCurrent(untouchable);
}

void AbstractScaningWidget::manualConfirmed()
{
	barcodeConfirmed(barcodeInput->text());
}
QString AbstractScaningWidget::transformModeToString(Modes m)
{
	switch (m)
	{
	case Modes::Search:
		return tr("Search");
	case Modes::Inventory:
		return tr("Inventory");
	case Modes::Supplies:
		return tr("Supplies");
	case Modes::Prices:
		return tr("Prices");
	case Modes::Simple:
		return tr("Simple");
	default:
		return "";
	}
}

void AbstractScaningWidget::_pushToHistory(Entity barcode)
{
	if (barcodeModel != Q_NULLPTR)
	{
		barcodeModel->addToDataEntity(barcode, 0);
	}
}

QString AbstractScaningWidget::_extractionCheck(QString barcode)
{
	if (AppSettings->extrasearchPrefix.isEmpty())
		return barcode;
	else
	{
		if (barcode.startsWith(AppSettings->extrasearchPrefix))
		{
			QString::const_iterator point = barcode.begin() + AppSettings->extrasearchPrefix.count();
			bool allzeros = true;
			int shuffle = 0;
			while (point != barcode.end() && shuffle < 5)
			{
				QChar sym = (*(point + shuffle++));
				allzeros &= (sym == '0');
			}
			QString buffer;
			if (allzeros)
			{
				buffer.reserve(10);
				point += 5;
				while (point < barcode.end())
				{
					buffer += (*(point++));
				}
			}
			else
			{
				buffer.resize(5);
				std::copy(point, point + 5, buffer.begin());
			}
			if (!buffer.isEmpty())
			{
				return buffer;
			}
			return barcode;
			
		}
		else
		{
			return barcode;
		}
	}
}

bool checkBarcodeIntegrity(const QString& bc)
//	Realization of checking. No null barcodes, no too long
{
	if (bc.isEmpty() || bc.length() > 100)
	{
		return false;
	}
	return true;
}
