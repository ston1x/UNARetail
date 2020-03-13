#include "PriceBranchWidget.h"
#include "PriceScaningWidget.h"

PriceBranchWidget::PriceBranchWidget( QWidget* parent)
	: BranchRootWidget(Modes::Prices, parent)
{
	pageName->setText(tr("supplies-widget-title_Supplies"));
	
}

inframedWidget* PriceBranchWidget::_allocateSettings()
{
	innerSettingsForm* insf = new innerSettingsForm(currentMode, this);
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
	return insf;
}

inframedWidget* PriceBranchWidget::_allocateScaning()
{
	PriceScaningWidget* scanwidget = new PriceScaningWidget(this);
	QObject::connect(scanwidget, &inframedWidget::backRequired, this, &BranchRootWidget::backRequire);
	QObject::connect(scanwidget, &PriceScaningWidget::barcodeReceived, 
		this, &BranchRootWidget::newBarcodeArrived);
	return scanwidget;
}

inframedWidget* PriceBranchWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
	return s;
}
