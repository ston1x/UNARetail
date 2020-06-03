#include "SimpleBranchWidget.h"

SimpleBranchWidget::SimpleBranchWidget( QWidget* parent)
	: BranchRootWidget( Modes::Simple, parent)
{
	pageName->setText(tr("simple_widget_title_Simple\n") + QString::number(
		AppSettings->getModeDescription(Modes::Simple).getSysfeed()));
}

void SimpleBranchWidget::backRequire()
{
	BranchRootWidget::backRequire();
	pageName->setText(tr("simple_widget_title_Simple\n") + QString::number(AppSettings->getModeDescription(Modes::Simple).getSysfeed()));
}


inframedWidget* SimpleBranchWidget::_allocateSettings()
{
	innerSettingsForm* insf = new innerSettingsForm(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(insf, &innerSettingsForm::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(insf, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return insf;
}

inframedWidget* SimpleBranchWidget::_allocateScaning()
{
	ScaningWidget* scanwidget = new ScaningWidget( currentMode, this);
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

inframedWidget* SimpleBranchWidget::_allocateStorage()
{
	ScanedStorageWidget* s = new ScanedStorageWidget(currentMode, this);
#ifdef QT_VERSION5X
	QObject::connect(s, &ScanedStorageWidget::backRequired, this, &BranchRootWidget::backRequire);
#else
	QObject::connect(s, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif
	return s;
}
