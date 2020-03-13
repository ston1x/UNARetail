#include "PriceScaningWidget.h"
#include <QKeyEvent>
#include <algorithm>
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>
#include "PricedBarcodeRedacting.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

PriceScaningWidget::PriceScaningWidget(QWidget* parent)
	: AbstractScaningWidget(Modes::Prices, parent),
	lengthCounter(new QLabel(untouchable)),
	totalCounter(new QLabel(untouchable)),
	generalPrice(new QSpinBox(untouchable)),
	discountPrice(new QSpinBox(untouchable)),
	okButton(new MegaIconButton(untouchable)), 
	pendingBarcode(new PricedBarcodeEntity())
{
	counterLayout->addWidget(lengthCounter);
	counterLayout->addWidget(totalCounter);

	controlLayout->addWidget(generalPrice);
	controlLayout->addWidget(discountPrice);

	buttonLayout->addWidget(okButton);

	okButton->setIcon(QIcon(":/res/submit.png"));

	okButton->setStyleSheet(QString(OK_BUTTONS_STYLESHEET));

	lengthCounter->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	totalCounter->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	
	QObject::connect(okButton, &QPushButton::clicked, this, &PriceScaningWidget::okPressed);
	QObject::connect(generalPrice, &QSpinBox::editingFinished, discountPrice, 
		QOverload<>::of(&QSpinBox::setFocus));
	QObject::connect(discountPrice, &QSpinBox::editingFinished, this, &PriceScaningWidget::barcodeReady);
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
	pendingBarcode->generalPrice = generalPrice->value();
	pendingBarcode->discountPrice = discountPrice->value();
	if (pendingBarcode->isValid())
	{
		emit barcodeReceived(pendingBarcode);
	}
}

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
}

void PriceScaningWidget::okPressed()
//	submits data to connected widget
{
	emit backRequired();
}
