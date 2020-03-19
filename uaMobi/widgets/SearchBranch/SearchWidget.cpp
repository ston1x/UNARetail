#include "SearchWidget.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif


SearchWidget::SearchWidget(QWidget* parent)
	: AbstractScaningWidget( Modes::Search, parent)
{
}

void SearchWidget::handleScanButton()
{
	
}

void SearchWidget::handleValueFromKeyboard(QString value)
{
	_emplaceBarcode(value);
}

void SearchWidget::barcodeReady()
{

}

void SearchWidget::_emplaceBarcode(QString barcode)
{
	if (barcode.isEmpty())
		return;
	barcodeInfo->setText(AppData->barcodeInfo(barcode));
}
#ifdef CAMERA_SUPPORT
void SearchWidget::handleCameraBarcode(QString value)
{
	_emplaceBarcode(value);
}
#endif

void SearchWidget::_clearControls()
{
}
