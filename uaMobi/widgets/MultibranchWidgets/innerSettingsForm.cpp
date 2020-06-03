#include "innerSettingsForm.h"
#include "widgets/utils/ElementsStyles.h"
#include <qmessagebox.h>
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/SettingsWidgets/ScaningSettings.h"
#include "widgets/SettingsWidgets/DatabaseSettings.h"
#include "widgets/SettingsWidgets/ExtraScaningSettings.h"
#include "widgets/SettingsWidgets/TaxInvoiceSettings.h"
#include <QTabWidget>
#include <QLabel>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qboxlayout.h>



innerSettingsForm::innerSettingsForm(Modes mode, QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	innerWidget(new QTabWidget(this)), scaningSettings(new ScaningSettings(this)),
	databaseSettings(new DatabaseSettings(mode, this)),
	extrascanSettings(new ExtraScaningSettings(mode, this)),
	taxInvoiceSettings(new TaxInvoiceSettings(mode,this)),
	backButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->addTab(scaningSettings, tr("Scaning"));
	innerWidget->addTab(databaseSettings, tr("Database"));
	innerWidget->addTab(extrascanSettings, tr("Extra"));
	innerWidget->addTab(taxInvoiceSettings, tr("TaxInv"));
	mainLayout->addWidget(backButton);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	this->setFont(AppGenFont);


	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setText(tr("back"));
#ifdef QT_VERSION5X
	QObject::connect(backButton, &QPushButton::pressed, this, &innerSettingsForm::dumpAndReturn);
#else
	QObject::connect(backButton, SIGNAL(pressed()), this, SLOT(dumpAndReturn()));
#endif
}

void innerSettingsForm::dumpAndReturn()
{
	databaseSettings->extractAndSave();
	scaningSettings->extractAndSave();
	extrascanSettings->extractAndSave();
	taxInvoiceSettings->extractAndSave();
	AppSettings->Save();
	emit backRequired();
}
