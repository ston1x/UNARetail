#include "ScaningWidget.h"
#include <QKeyEvent>
#include <algorithm>
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>
#include "widgets/utils/GlobalAppSettings.h"
#include <QInputMethod>
#ifdef DEBUG
#include "debugtrace.h"
#endif

ScaningWidget::ScaningWidget(Modes mode, QWidget* parent)
	: AbstractScaningWidget(mode, parent),
	lengthInfo(new TwoLevelCounterLabel(tr("scaning_widget_TOTAL:"), NAN, untouchable)),
	totalInfo(new TwoLevelCounterLabel(tr("scaning_widget_LEN:"), NAN, untouchable)),
	okButton(new MegaIconButton(untouchable)),
	quantityControl(new QuantityControl(   
		(!AppSettings->floatControl[int(mode)])
		, tr("Quantity"), untouchable)),
	pendingBarcode(new BarcodeEntity())
{
	counterLayout->addWidget(lengthInfo);
	counterLayout->addWidget(totalInfo);
	buttonLayout->addWidget(okButton);
	
	
	okButton->setIcon(QIcon(":/res/submit.png"));

	okButton->setStyleSheet(QString(OK_BUTTONS_STYLESHEET));

	totalInfo->show();
	lengthInfo->show();

	lengthInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	totalInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
#ifdef Q_OS_WINCE
	lengthInfo->setFont(AppFonts->makeFont(1.5));
	totalInfo->setFont(AppFonts->makeFont(1.5));
#else
	lengthInfo->setFont(AppFonts->makeFont(2.5));
	totalInfo->setFont(AppFonts->makeFont(2.5));
#endif
	totalInfo->setMinimumHeight(calculateAdaptiveButtonHeight());
	lengthInfo->setMinimumHeight(calculateAdaptiveButtonHeight());
	
	if (!AppSettings->navigationElements)
	{
		okButton->hide();
	}

	controlLayout->addWidget(quantityControl->myWidget());

	quantityControl->show();
#ifdef QT_VERSION5X
	QObject::connect(okButton, &QPushButton::clicked, this, &ScaningWidget::okPressed);
	QObject::connect(quantityControl, &abs_control::editingFinished, this, &ScaningWidget::barcodeReady);
#ifdef Q_OS_ANDROID
    QObject::connect(quantityControl, &abs_control::editingFinished, qApp->inputMethod(),&QInputMethod::hide);
#endif
#else
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
	QObject::connect(quantityControl, SIGNAL(editingFinished()), this, SLOT(barcodeReady()));
#endif
}



void ScaningWidget::handleScanButton()
// handles Scan button, inserts data into all inner lists
{
	if (!pendingBarcode->barcode.isEmpty())
	{
		bool ok = false;
		pendingBarcode->quantity = quantityControl->getValue().toInt(&ok);
		if (!ok)
			return;
		barcodeReady();
	}
}
void ScaningWidget::setTotal(int total)
{
	totalInfo->setValue(total);
}

void ScaningWidget::setLen()
{
	lengthInfo->setValue(pendingBarcode->barcode.length());
}

void ScaningWidget::handleValueFromKeyboard(QString val)
{
	if (barcodeInput->text().isEmpty())
	{
		barcodeInput->setText(val);
        hideCurrent();
		_emplaceBarcode(val);
		_clearControls();
	}
	else
	{
        hideCurrent();
		quantityControl->setValue(val);
		barcodeReady();
		barcodeInput->clear();
	}
}
void ScaningWidget::barcodeReady()
{
	pendingBarcode->quantity = quantityControl->getPureValue();
	if (pendingBarcode->isValid())
	{
		pendingBarcode->comment = barcodeInfo->toPlainText();
		_pushToHistory(pendingBarcode);
		emit barcodeReceived(pendingBarcode);
		quantityControl->setValue("0");
		setTotal(AppData->sumAllFilteredIn(currentMode, pendingBarcode->barcode,
			BarcodeEntity::getEnumerableFieldIndex(), pendingBarcode, TableNames::Scanned));
		if (AppSettings->clearScanBuffer)
		{
			pendingBarcode.clear();
			pendingBarcode = Barcode(new BarcodeEntity());
			barcodeInput->clear();
			barcodeInfo->clear();
			totalInfo->clear();
			lengthInfo->clear();
		}
		else
		{
			QString tname = pendingBarcode->getName();
			QString tcomment = pendingBarcode->comment;
			pendingBarcode.clear();
			pendingBarcode = Barcode(new BarcodeEntity(tname, tcomment));
		}
	}
}
#ifdef CAMERA_SUPPORT
void ScaningWidget::handleCameraBarcode(QString value)
{
	if (!pendingBarcode->barcode.isEmpty() && !quantityControl->canGiveValue())
	{
		quantityControl->setValue("1");
	}
	barcodeReady();
    hideCurrent();
	_emplaceBarcode(value);
}
#endif
void ScaningWidget::okPressed()
//	submits data to connected widget
{
	barcodeReady();
	if (!AppSettings->clearScanBuffer)
		emit backRequired();
}

void ScaningWidget::_emplaceBarcode(QString barcode)
{
	if (!barcode.isEmpty())
	{
		barcode = _extractionCheck(barcode);
        pendingBarcode.clear();
        pendingBarcode = Barcode(new BarcodeEntity(barcode));
		quantityControl->setFocus();
		quantityControl->setValue("0");
		barcodeInput->setText(barcode);
		if (AppSettings->autoSearch)
		{
			ShortBarcode info  = upcastEntity<ShortBarcodeEntity>(AppData->barcodeInfo(barcode));
			if (info != Q_NULLPTR)
			{
				barcodeInfo->setText(info->info);
				pendingBarcode->comment = info->info;
			}
			else
			{
				barcodeInfo->clear();
			}
		}
		setLen();
		setTotal(AppData->sumAllFilteredIn(currentMode, barcode, BarcodeEntity::getEnumerableFieldIndex(), pendingBarcode, TableNames::Scanned));
	
	}
}

void ScaningWidget::_clearControls()
{
	quantityControl->setValue(0);
}
