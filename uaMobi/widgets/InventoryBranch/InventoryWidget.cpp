#include "InventoryWidget.h"

InventoryWidget::InventoryWidget(QWidget* parent)
	: BranchRootWidget(Modes::Inventory, parent)
{
	pageName->setText(tr("inventory_title_Inventory"));
}

inframedWidget* InventoryWidget::_allocateSettings()
{
	innerSettingsForm* insf = new innerSettingsForm(currentMode ,this);
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
	return insf;
}

inframedWidget* InventoryWidget::_allocateScaning()
{
	ScaningWidget* scanwidget = new ScaningWidget(currentMode, this);
	QObject::connect(scanwidget, &inframedWidget::backRequired,
		this, &BranchRootWidget::backRequire);
	QObject::connect(scanwidget, &ScaningWidget::barcodeReceived,
		this, &BranchRootWidget::newBarcodeArrived);
	return scanwidget;
}

inframedWidget* InventoryWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
	return s;
}

