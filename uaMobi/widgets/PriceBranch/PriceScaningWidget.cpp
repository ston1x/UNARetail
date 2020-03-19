#include "PriceScaningWidget.h"
#include <QKeyEvent>
#include <algorithm>
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>
#include "PricedBarcodeRedacting.h"

#ifdef DEBUG
#include "debugtrace.h"
#endif

PriceScaningWidget::PriceScaningWidget(QWidget* parent)
	: AbstractScaningWidget(Modes::Prices, parent),
	lengthCounter(new QLabel(untouchable)),
	totalCounter(new QLabel(untouchable)),
	generalPrice(new QuantityControl(false, tr("General"), untouchable)),
	discountPrice(new QuantityControl(false, tr("Discount"), untouchable)),
	okButton(new MegaIconButton(untouchable)), 
	pendingBarcode(new PricedBarcodeEntity())
{
	counterLayout->addWidget(lengthCounter);
	counterLayout->addWidget(totalCounter);

	controlLayout->addWidget(generalPrice->myWidget());
	controlLayout->addWidget(discountPrice->myWidget());

	buttonLayout->addWidget(okButton);

	okButton->setIcon(QIcon(":/res/submit.png"));

	okButton->setStyleSheet(QString(OK_BUTTONS_STYLESHEET));

	lengthCounter->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	totalCounter->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	lengthCounter->setFont(AppFonts->makeFont(2.5));
	totalCounter->setFont(AppFonts->makeFont(2.5));
	totalCounter->setMinimumHeight(calculateAdaptiveButtonHeight());
	lengthCounter->setMinimumHeight(calculateAdaptiveButtonHeight());

	if (!AppSettings->navigationElements)
	{
		okButton->hide();
	}

	generalPrice->show();
	discountPrice->show();
	
	QObject::connect(okButton, &QPushButton::clicked, this, &PriceScaningWidget::okPressed);
	QObject::connect(generalPrice, &QuantityControl::editingFinished, discountPrice, 
		&QuantityControl::setFocus);
	QObject::connect(discountPrice, &QuantityControl::editingFinished, this, &PriceScaningWidget::barcodeReady);
}

void PriceScaningWidget::show()
{
	AbstractScaningWidget::show();
}

void PriceScaningWidget::_emplaceBarcode(QString barcode)
{
	if (!barcode.isEmpty())
	{
		pendingBarcode.reset(new PricedBarcodeEntity(barcode));
		generalPrice->setFocus();
		barcodeInput->setText(barcode);
		if (AppSettings->autoSearch)
			barcodeInfo->setText(AppData->barcodeInfo(barcode));
		setLen();
		setTotal(AppData->sumAllIn(currentMode, BarcodeEntity::getEnumerableFieldIndex(), pendingBarcode, TableNames::Scanned));
	}
}

void PriceScaningWidget::_clearControls()
{
	generalPrice->setValue(0);
	discountPrice->setValue(0);
}

void PriceScaningWidget::handleScanButton()
{
	if (pendingBarcode->isValid())
	{
		emit barcodeReceived(pendingBarcode);
		barcodeInput->clear();
		pendingBarcode.reset(new PricedBarcodeEntity());
	}
}

void PriceScaningWidget::barcodeReady()
{
	pendingBarcode->generalPrice = generalPrice->getValue().toDouble();
	pendingBarcode->discountPrice = discountPrice->getValue().toDouble();
	if (pendingBarcode->isValid())
	{
		emit barcodeReceived(pendingBarcode);
		generalPrice->setValue("0");
		discountPrice->setValue("0");
	}
	setTotal(AppData->countAllIn(currentMode, pendingBarcode, TableNames::Scanned));
}
#ifdef CAMERA_SUPPORT
void PriceScaningWidget::handleCameraBarcode(QString value)
{
	_emplaceBarcode(value);
	_clearControls();
}
#endif
void PriceScaningWidget::setLen()
{
	lengthCounter->setText(tr("price_scaning_widget_LEN:") + " |" + QString::number(pendingBarcode->barcode.length()));
}

void PriceScaningWidget::setTotal(int total)
{
	totalCounter->setText(tr("TOTAL: ") + QString::number(total));
}


void PriceScaningWidget::handleValueFromKeyboard(QString val)
{
	if (barcodeInput->text().isEmpty())
	{
		barcodeInput->setText(val);
	}
	else
	{
		if (!generalPrice->canGiveValue())
		{
			generalPrice->setValue(val);
			discountPrice->setFocus();
		}
		else
		{
			discountPrice->setValue(val);
			barcodeReady();
			barcodeInput->clear();
		}
	}
}

void PriceScaningWidget::okPressed()
//	submits data to connected widget
{
	emit backRequired();
}
