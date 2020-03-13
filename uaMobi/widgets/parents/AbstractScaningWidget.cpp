#include "AbstractScaningWidget.h"
#include <QtWidgets/QScroller>
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>
#ifdef DEBUG
#include "debugtrace.h"
#endif


AbstractScaningWidget::AbstractScaningWidget(Modes mode, QWidget* parent)
	: inframedWidget(parent), abstractDynamicNode(new inframedWidget(this), new QVBoxLayout(this)),
	innerLayout(new QVBoxLayout(untouchable)), counterLayout(new QHBoxLayout(untouchable)),
	controlLayout(new QVBoxLayout(untouchable)),
	buttonLayout(new QHBoxLayout(untouchable)),
	additionalInputLayout(new QHBoxLayout(untouchable)),
	modeName(new QLabel(untouchable)),
	barcodeInfo(new QLabel(untouchable)),
	barcodeInput(new QLineEdit(untouchable)),
	backButton(new MegaIconButton(untouchable)),
	keyboardButton(new MegaIconButton(untouchable)), cameraButton(new MegaIconButton(untouchable)),
	currentMode(mode)
{
	this->setLayout(mainLayout);
	untouchable->setLayout(innerLayout);
	innerLayout->addWidget(modeName);
	innerLayout->addWidget(barcodeInput);
	innerLayout->addWidget(barcodeInfo);
	innerLayout->addLayout(counterLayout);
	innerLayout->addStretch(0);
	innerLayout->addLayout(controlLayout);
	innerLayout->addStretch(0);
	innerLayout->addLayout(additionalInputLayout);
	innerLayout->addLayout(buttonLayout);
	
	
	buttonLayout->addWidget(backButton);
	additionalInputLayout->addWidget(keyboardButton);
	additionalInputLayout->addWidget(cameraButton);

	// Layouts are reversal so that new elements were pushed to left
	buttonLayout->setDirection(QBoxLayout::RightToLeft);

	// No any spacings or margins
	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	// Set up view
	backButton->setIcon(QIcon(":/res/back.png"));
	cameraButton->setIcon(QIcon(":/res/camera.png"));
	keyboardButton->setIcon(QIcon(":/res/key.png"));

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	barcodeInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	barcodeInfo->setMinimumHeight(calculateAdaptiveButtonHeight());
	barcodeInfo->setFont(AppFonts->makeFont(3.0));
	barcodeInfo->setWordWrap(true);
	barcodeInfo->setAlignment(Qt::AlignCenter);

	modeName->setFont(AppFonts->makeFont(2.0));
	modeName->setAlignment(Qt::AlignCenter);
	modeName->setText(transformModeToString(currentMode));

	barcodeInput->setMinimumHeight(calculateAdaptiveButtonHeight());
	barcodeInput->setFont(AppFonts->makeFont(3.0));
	barcodeInput->setAlignment(Qt::AlignCenter);

	if (!AppSettings->additionalControlElements)
	{
		cameraButton->hide();
		keyboardButton->hide();
	}
	if (!AppSettings->navigationElements)
	{
		backButton->hide();
	}

	BarcodeObs->activate();
	
	QObject::connect(backButton, &QPushButton::clicked, this, &AbstractScaningWidget::backRequired);
	QObject::connect(keyboardButton, &MegaIconButton::clicked, this, &AbstractScaningWidget::keyboardRequired);
	QObject::connect(cameraButton, &QPushButton::clicked, this, &AbstractScaningWidget::cameraRequired);
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(BarcodeObs, &BarcodeObserver::scanButtonPress, this, &AbstractScaningWidget::handleScanButton);
	QObject::connect(barcodeInput, &QLineEdit::returnPressed, this, &AbstractScaningWidget::manualConfirmed);
}

void AbstractScaningWidget::show()
{
	if (AppSettings->additionalControlElements)
	{
		keyboardButton->show();
		cameraButton->show();
	}
	else
	{
		keyboardButton->hide();
		cameraButton->hide();
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

void AbstractScaningWidget::cameraRequired()
//	This slot creates new camera widget because only one camera widget can exist
{
	_hideAndDeleteCurrent(new ScaningCameraWidget(this));
	QObject::connect(_upCO<ScaningCameraWidget>(), &ScaningCameraWidget::backRequired, this, &AbstractScaningWidget::hideCurrent);
	QObject::connect(_upCO<ScaningCameraWidget>(), &ScaningCameraWidget::hasBarcode, this, &AbstractScaningWidget::barcodeConfirmed);
}

void AbstractScaningWidget::keyboardRequired()
//	This slot creates new keyboard
{
	_hideAndDeleteCurrent(new VirtualBarcodeKeyboard(this));
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::backRequired, this, &AbstractScaningWidget::hideCurrent);
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::barcodeConfirmed, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(_upCO<VirtualBarcodeKeyboard>(), &VirtualBarcodeKeyboard::valueConfirmed, this, &AbstractScaningWidget::handleValueFromKeyboard);
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

bool checkBarcodeIntegrity(const QString& bc)
//	Realization of checking. No null barcodes, no too long
{
	if (bc.isEmpty() || bc.length() > 100)
	{
		return false;
	}
	return true;
}