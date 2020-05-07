#include "SuppliesWidget.h"

SuppliesWidget::SuppliesWidget(QWidget* parent) :
	BranchRootWidget(Modes::Supplies, parent)
{
	pageName->setText(tr("supplies-widget-title_Supplies"));
}

inframedWidget* SuppliesWidget::_allocateSettings()
{
	innerSettingsForm* insf = new innerSettingsForm(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(insf, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return insf;
}

inframedWidget* SuppliesWidget::_allocateScaning()
{
	ScaningWidget* scanwidget = new ScaningWidget(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(scanwidget, &inframedWidget::backRequired, this, &BranchRootWidget::backRequire);
	QObject::connect(scanwidget, &ScaningWidget::barcodeReceived, this, &BranchRootWidget::newBarcodeArrived);
#else
	QObject::connect(scanwidget, SIGNAL(backRequired()), this, SLOT(backRequire()));
    QObject::connect(scanwidget, SIGNAL(barcodeReceived(Entity)), this, SLOT(newBarcodeArrived(Entity)));
#endif
	return scanwidget;
}

inframedWidget* SuppliesWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
#else
    QObject::connect(s, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return s;
}
