#include "SimpleBranchWidget.h"

SimpleBranchWidget::SimpleBranchWidget( QWidget* parent)
	: BranchRootWidget( Modes::Simple, parent)
{
	pageName->setText(tr("simple_widget_title_Simple"));
}

inframedWidget* SimpleBranchWidget::_allocateSettings()
{
	innerSettingsForm* insf = new innerSettingsForm(currentMode, this);
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
	return insf;
}

inframedWidget* SimpleBranchWidget::_allocateScaning()
{
	ScaningWidget* scanwidget = new ScaningWidget( currentMode, this);
	QObject::connect(scanwidget, &inframedWidget::backRequired,
		this, &BranchRootWidget::backRequire);
	QObject::connect(scanwidget, &ScaningWidget::barcodeReceived,
		this, &BranchRootWidget::newBarcodeArrived);
	return scanwidget;
}

inframedWidget* SimpleBranchWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
	return s;
}
