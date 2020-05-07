#include "SearchWidget.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif


SearchWidget::SearchWidget(QWidget* parent)
	: AbstractScaningWidget( Modes::Search, parent)
{
	if (historyView != Q_NULLPTR)
		historyView->hide();
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
	Entity e = AppData->barcodeInfo(barcode);
	if (e != Q_NULLPTR)
		barcodeInfo->setText(e->maximizedView("|", dateDBEncoding));
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
