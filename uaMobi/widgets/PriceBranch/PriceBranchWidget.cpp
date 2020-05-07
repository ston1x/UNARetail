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
#ifdef QT_VERSION5X
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(insf, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return insf;
}

inframedWidget* PriceBranchWidget::_allocateScaning()
{
	PriceScaningWidget* scanwidget = new PriceScaningWidget(this);
#ifdef QT_VERSION5X
	QObject::connect(scanwidget, &inframedWidget::backRequired, this, &BranchRootWidget::backRequire);
	QObject::connect(scanwidget, &PriceScaningWidget::barcodeReceived, 
		this, &BranchRootWidget::newBarcodeArrived);
#else
	QObject::connect(scanwidget, SIGNAL(backRequired()), this, SLOT(backRequire()));
	QObject::connect(scanwidget, SIGNAL(barcodeReceived(Entity)),
		this, SLOT(newBarcodeArrived(Entity)));
#endif
	return scanwidget;
}

inframedWidget* PriceBranchWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(s, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return s;
}
