#include "SuppliesWidget.h"

SuppliesWidget::SuppliesWidget(QWidget* parent) :
	BranchRootWidget(Modes::Supplies, parent)
{
	pageName->setText(tr("supplies-widget-title_Supplies"));
}

inframedWidget* SuppliesWidget::_allocateSettings()
{
	innerSettingsForm* insf = new innerSettingsForm(currentMode, this);
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
	return insf;
}

inframedWidget* SuppliesWidget::_allocateScaning()
{
	ScaningWidget* scanwidget = new ScaningWidget(currentMode, this);
	QObject::connect(scanwidget, &inframedWidget::backRequired, this, &BranchRootWidget::backRequire);
	QObject::connect(scanwidget, &ScaningWidget::barcodeReceived, this, &BranchRootWidget::newBarcodeArrived);
	return scanwidget;
}

inframedWidget* SuppliesWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
	return s;
}
