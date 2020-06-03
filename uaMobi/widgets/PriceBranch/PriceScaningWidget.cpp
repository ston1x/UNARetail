#include "PriceScaningWidget.h"
#include <QKeyEvent>
#include <algorithm>
#include "widgets/utils/ElementsStyles.h"
#include <QtCore/QTimer>
#include <QInputMethod>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/utils/GlobalAppSettings.h"
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
#ifdef Q_OS_WINCE
	lengthCounter->setFont(AppFonts->makeFont(1.5));
	totalCounter->setFont(AppFonts->makeFont(1.5));
#else
	lengthCounter->setFont(AppFonts->makeFont(2.5));
	totalCounter->setFont(AppFonts->makeFont(2.5));
#endif
	totalCounter->setMinimumHeight(calculateAdaptiveButtonHeight());
	lengthCounter->setMinimumHeight(calculateAdaptiveButtonHeight());

	if (!AppSettings->navigationElements)
	{
		okButton->hide();
	}

	generalPrice->show();
	discountPrice->show();
#ifdef QT_VERSION5X
	QObject::connect(okButton, &QPushButton::clicked, this, &PriceScaningWidget::okPressed);
	if (!QObject::connect(barcodeInfo, &ReturnableTextEdit::returnPressed, generalPrice, &abs_control::setFocus))
		barcodeInfo->debugLine += "CERR";
	QObject::connect(generalPrice, &QuantityControl::editingFinished, discountPrice, 
		&QuantityControl::setFocus);
	QObject::connect(discountPrice, &QuantityControl::editingFinished, this, &PriceScaningWidget::barcodeReady);
#ifdef Q_OS_ANDROID
    QObject::connect(discountPrice, &QuantityControl::editingFinished, qApp->inputMethod(), &QInputMethod::hide);
#endif
#else
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
	QObject::connect(generalPrice, SIGNAL(editingFinished()), discountPrice,
		SLOT(setFocus()));
	QObject::connect(discountPrice, SIGNAL(editingFinished()), this, SLOT(barcodeReady()));
#endif
}

void PriceScaningWidget::show()
{
	AbstractScaningWidget::show();
}

void PriceScaningWidget::_emplaceBarcode(QString barcode, ShortBarcode info)
{
	if (!barcode.isEmpty())
	{
		barcode = _extractionCheck(barcode);
        pendingBarcode.clear();
        pendingBarcode = PricedBarcode(new PricedBarcodeEntity(barcode));
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
		setTotal(AppData->sumAllIn(currentMode, 3, pendingBarcode, TableNames::Scanned));
		if (barcodeInfo->toPlainText().isEmpty())
            barcodeInfo->setFocus(Qt::MouseFocusReason);
		else
            generalPrice->setFocus();
	}
}

void PriceScaningWidget::_clearControls()
{
    generalPrice->setValue(QString("0"));
    discountPrice->setValue(QString("0"));
}

void PriceScaningWidget::_pushToHistory(Entity e)
{
	if (barcodeModel != Q_NULLPTR)
	{
		barcodeModel->addToDataEntity(e, 0);
		barcodeModel->addToDataEntity(e, 1);
	}
}

void PriceScaningWidget::handleScanButton()
{
	if (pendingBarcode->isValid())
	{
		emit barcodeReceived(pendingBarcode);
		barcodeInput->clear();
        pendingBarcode.clear();
        pendingBarcode = PricedBarcode(new PricedBarcodeEntity());
	}
}

void PriceScaningWidget::barcodeReady()
{
	pendingBarcode->generalPrice = generalPrice->getValue().toDouble();
	pendingBarcode->discountPrice = discountPrice->getValue().toDouble();
	if (pendingBarcode->isValid())
	{
		pendingBarcode->comment = barcodeInfo->toPlainText();
		_pushToHistory(pendingBarcode);
		emit barcodeReceived(pendingBarcode);
        QString tname = pendingBarcode->barcode;
        pendingBarcode.clear();
        pendingBarcode = PricedBarcode(new PricedBarcodeEntity(tname));
		generalPrice->setValue("0");
		discountPrice->setValue("0");
		if (AppSettings->clearScanBuffer)
		{
			pendingBarcode.clear();
			pendingBarcode = PricedBarcode(new PricedBarcodeEntity());
			barcodeInput->clear();
			barcodeInfo->clear();
			totalCounter->clear();
			lengthCounter->clear();
            barcodeInput->setFocus(Qt::FocusReason::MouseFocusReason);

		}
		else
		{
			setTotal(AppData->countAllIn(currentMode, pendingBarcode, TableNames::Scanned));
		}
	}
	

}
#ifdef CAMERA_SUPPORT
void PriceScaningWidget::handleCameraBarcode(QString value)
{
    hideCurrent();
    update();
    qApp->processEvents();
    barcodeReady();
    barcodeConfirmed(value);
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
    barcodeReady();
	if (!AppSettings->clearScanBuffer)
		emit backRequired();
}
