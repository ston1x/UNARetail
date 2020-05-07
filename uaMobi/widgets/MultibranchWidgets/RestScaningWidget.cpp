#include "RestScaningWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "externalCommunication/AsyncRequestEngine.h"



void RestScaningWidget::_emplaceBarcode(QString barcode)
{
	if (!barcode.isEmpty())
	{
		barcode = _extractionCheck(barcode);
		pendingBarcode.clear();
		itemCode.clear();
		pendingBarcode = Barcode(new BarcodeEntity(barcode));
		quantityControl->setFocus();
		quantityControl->setValue("0");
		barcodeInput->setText(barcode);
		if (AppSettings->autoSearch)
		{
			ShortBarcode info = upcastEntity<ShortBarcodeEntity>(AppData->barcodeInfo(barcode));
			if (info != Q_NULLPTR)
			{
				barcodeInfo->setText(info->info);
				pendingBarcode->comment = info->info;
				itemCode = QString::number(info->code);
			}
			else
			{
				barcodeInfo->clear();
			}
		}
		setLen();
		setTotal(AppData->sumAllFilteredIn(currentMode, barcode, BarcodeEntity::getEnumerableFieldIndex(), pendingBarcode, TableNames::Scanned));
		sendRestRequest();
	}
}

void RestScaningWidget::operateOverResponse(QStringList parsedResponse)
{
	if (parsedResponse.isEmpty())
		return;
	bool ok;
	double rest = parsedResponse.first().toDouble(&ok);
	if (ok)
	{
		restLabel->setValue(rest);
	}
	else
	{
		restLabel->setValue(NAN);
	}
}

RestScaningWidget::RestScaningWidget(Modes mode, QWidget* parent)
	: UpdateableScaningWidget(mode,parent), itemCode(), 
	restLabel(new ClickableTLCounterLabel(tr("Rest"), NAN,this)),
	requestTemplate("?c=get_sld&username=%1&barcode=%2&code=%3&place=%4&sysfid=%5")
{
	counterLayout->addWidget(restLabel);
	QObject::connect(restLabel, &ClickableTLCounterLabel::clicked, this, &RestScaningWidget::sendRestRequest);
}

void RestScaningWidget::sendRestRequest()
{
	datarefreshEngine->clearAndRequest(
		requestTemplate.arg(
			AppSettings->userLogin
		).arg(
			pendingBarcode->barcode
		).arg(
			itemCode
		).arg(
			AppSettings->placeAsCode
		).arg(
			AppSettings->sysfeed.at(int(Modes::Invoices))
		)
	);
}
