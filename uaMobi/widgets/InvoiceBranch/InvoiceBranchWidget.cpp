#include "InvoiceBranchWidget.h"
#include "widgets/MultibranchWidgets/RestScaningWidget.h"


inframedWidget* InvoiceBranchWidget::_allocateScaning()
{
	RestScaningWidget* scanwidget = new RestScaningWidget(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(scanwidget, &inframedWidget::backRequired,
		this, &BranchRootWidget::backRequire);
	QObject::connect(scanwidget, &ScaningWidget::barcodeReceived,
		this, &BranchRootWidget::newBarcodeArrived);
#else
	QObject::connect(scanwidget, SIGNAL(backRequired()),
		this, SLOT(backRequire()));
	QObject::connect(scanwidget, SIGNAL(barcodeReceived(Entity)),
		this, SLOT(newBarcodeArrived(Entity)));
#endif
	return scanwidget;
}

inframedWidget* InvoiceBranchWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(s, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return s;
}

inframedWidget* InvoiceBranchWidget::_allocateSettings()
{
	innerSettingsForm* insf = new innerSettingsForm(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(insf, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return insf;
}

InvoiceBranchWidget::InvoiceBranchWidget(QWidget* parent)
	: BranchRootWidget(Modes::Invoices, parent)
{
	pageName->setText(tr("simple_widget_title_Invoices\n") + QString::number(AppSettings->sysfeed.at(int(currentMode))));
}

void InvoiceBranchWidget::backRequire()
{
	BranchRootWidget::backRequire();
	pageName->setText(tr("simple_widget_title_Invoices\n") + QString::number(AppSettings->sysfeed.at(int(currentMode))));
}
