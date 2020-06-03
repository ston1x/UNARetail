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
#include <qmessagebox.h>
#include <cmath>
ScaningWidget::ScaningWidget(Modes mode, QWidget* parent)
	: AbstractScaningWidget(mode, parent),
    lengthInfo(new TwoLevelCounterLabel(tr("scaning_widget_LEN:"), qQNaN(), untouchable)),
    totalInfo(new TwoLevelCounterLabel(tr("scaning_widget_TOTAL:"), qQNaN(), untouchable)),
	okButton(new MegaIconButton(untouchable)),
	quantityControl(new QuantityControl(   
		!(AppSettings->getModeDescription(mode).requiresFloatControl())
		, tr("Quantity"), untouchable)),
	taxInvoiceInfo((AppSettings->getModeDescription(mode).isInsertingTaxInvoiceNumAllowed()) ? 
		(new QLabel(tr("tax invoice info"), this)) : Q_NULLPTR),
	taxInvoiceField(
		(AppSettings->getModeDescription(mode).isInsertingTaxInvoiceNumAllowed())? 
		new QLineEdit(this) : Q_NULLPTR
	),
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
	if (AppSettings->getModeDescription(mode).isInsertingTaxInvoiceNumAllowed())
	{
		controlLayout->addWidget(taxInvoiceInfo);
		taxInvoiceInfo->setAlignment(Qt::AlignCenter);
		controlLayout->addWidget(taxInvoiceField);
		taxInvoiceField->setText(AppSettings->taxInvoiceTemporary);
	}
	quantityControl->show();
#ifdef QT_VERSION5X
	QObject::connect(okButton, &QPushButton::clicked, this, &ScaningWidget::okPressed);
	QObject::connect(quantityControl, &abs_control::editingFinished, this, &ScaningWidget::barcodeReady);
	if (!QObject::connect(barcodeInfo, &ReturnableTextEdit::returnPressed, quantityControl, &abs_control::setFocus))
		barcodeInfo->debugLine += "CERR";
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
		_emplaceBarcode(val, _barcodeSearch(val));
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
		if (AppSettings->getModeDescription(currentMode).isInsertingTaxInvoiceNumAllowed())
		{
			if (AppSettings->getModeDescription(currentMode).isForbiddenInsertingWithoutTaxInvoice()
				&& taxInvoiceField->text().isEmpty())
			{
                taxInvoiceField->setStyleSheet(ERROR_LINEEDIT_STYLESHEET);
				return;
			}
			pendingBarcode->taxInvoiceNumber = taxInvoiceField->text();
			AppSettings->taxInvoiceTemporary = pendingBarcode->taxInvoiceNumber;
		}
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
			barcodeInput->setFocus();
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
bool ScaningWidget::_validateBarcode(QString bc)
{
	if (AppSettings->getModeDescription(currentMode).isInsertingTaxInvoiceNumAllowed())
	{
		bool startsWithAlpha = true;
		switch (bc.count())
		{
		default:
		case 2:
			startsWithAlpha &= bc.at(1).isLetter();
		case 1:
			startsWithAlpha &= bc.at(0).isLetter();
			break;
		case 0:
			startsWithAlpha = false;
		}
		if (startsWithAlpha)
		{
			taxInvoiceField->setText(bc);
			AppSettings->taxInvoiceTemporary = pendingBarcode->taxInvoiceNumber;
			if (AppSettings->getModeDescription(currentMode).isForbiddenInsertingWithoutTaxInvoice())
			{
				taxInvoiceField->setStyleSheet("");
			}
			return false;
		}
	}
	return AbstractScaningWidget::_validateBarcode(bc);
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
	barcodeConfirmed(value);
}
#endif
void ScaningWidget::okPressed()
//	submits data to connected widget
{
	barcodeReady();
	if (!AppSettings->clearScanBuffer)
		emit backRequired();
}

void ScaningWidget::_emplaceBarcode(QString barcode, ShortBarcode info)
{
	if (!barcode.isEmpty())
	{
		barcode = _extractionCheck(barcode);
        pendingBarcode.clear();
        pendingBarcode = Barcode(new BarcodeEntity(barcode));
		quantityControl->setFocus();
		if (AppSettings->autoFillQuantity)
			quantityControl->setValue("1");
		else
			quantityControl->setValue("0");
		barcodeInput->setText(barcode);
		if (info != Q_NULLPTR)
		{
			barcodeInfo->setText(info->info);
			pendingBarcode->comment = info->info;
		}
		else
		{
			barcodeInfo->clear();
		}
		setLen();
		setTotal(AppData->sumAllFilteredIn(currentMode, barcode, BarcodeEntity::getEnumerableFieldIndex(), pendingBarcode, TableNames::Scanned));
	
	}
}

void ScaningWidget::_clearControls()
{
	quantityControl->setValue(0);
}
