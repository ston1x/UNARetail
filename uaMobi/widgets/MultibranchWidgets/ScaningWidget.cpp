#include "ScaningWidget.h"
#include <QKeyEvent>
#include <algorithm>
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>

#ifdef DEBUG
#include "debugtrace.h"
#endif

ScaningWidget::ScaningWidget(Modes mode, QWidget* parent)
	: AbstractScaningWidget(mode, parent),
	lengthInfo(new QLabel(untouchable)),
	totalInfo(new QLabel(untouchable)),
	okButton(new MegaIconButton(untouchable)),
	quantityControl(new QuantityControl(true, tr("Quantity"), untouchable)),
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
	lengthInfo->setFont(AppFonts->makeFont(2.5));
	totalInfo->setFont(AppFonts->makeFont(2.5));
	totalInfo->setMinimumHeight(calculateAdaptiveButtonHeight());
	lengthInfo->setMinimumHeight(calculateAdaptiveButtonHeight());
	
	controlLayout->addWidget(quantityControl->myWidget());

	quantityControl->show();

	QObject::connect(okButton, &QPushButton::clicked, this, &ScaningWidget::okPressed);
	QObject::connect(quantityControl, &abs_control::editingFinished, this, &ScaningWidget::barcodeReady);
}



void ScaningWidget::handleScanButton()
// handles Scan button, inserts data into all inner lists
{
	if (pendingBarcode->isValid())
	{
		emit barcodeReceived(pendingBarcode);
		barcodeInput->clear();
		pendingBarcode.reset(new BarcodeEntity());
	}
}
void ScaningWidget::setTotal(int total)
{
	totalInfo->setText(tr("scaning_widget_TOTAL:") + " |" + QString::number(total));
}

void ScaningWidget::setLen()
{
	lengthInfo->setText(tr("scaning_widget_LEN:") + " |" + QString::number(pendingBarcode->barcode.length()));
}

void ScaningWidget::handleValueFromKeyboard(QString val)
{
	if (barcodeInput->text().isEmpty())
	{
		barcodeInput->setText(val);
	}
}
void ScaningWidget::barcodeReady()
{
	pendingBarcode->quantity = quantityControl->getValue().toDouble();
	if (pendingBarcode->isValid())
	{
		emit barcodeReceived(pendingBarcode);
		quantityControl->setValue("0");
	}
	setTotal(AppData->sumAllIn(currentMode, BarcodeEntity::getEnumerableFieldIndex(), pendingBarcode, TableNames::Scanned));
}

void ScaningWidget::okPressed()
//	submits data to connected widget
{
	emit backRequired();
}

void ScaningWidget::_emplaceBarcode(QString barcode)
{
	if (!barcode.isEmpty())
	{
		pendingBarcode.reset(new BarcodeEntity(barcode));
		quantityControl->setFocus();
		barcodeInput->setText(barcode);
		barcodeInfo->setText(AppData->barcodeInfo(barcode));
		setLen();
		setTotal(AppData->sumAllIn(currentMode, BarcodeEntity::getEnumerableFieldIndex(), pendingBarcode, TableNames::Scanned));

	}
}

void ScaningWidget::_clearControls()
{
	quantityControl->setValue(0);
}
