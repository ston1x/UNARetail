#include "SettingsForm.h"

#ifdef DEBUG
#include "debugtrace.h"

debugtrace downloadDebug("downloadlog.txt", detr_supply::all, detr_supply::file);
#endif

#include <QtWidgets/QMessageBox>
#include "widgets/utils/ElementsStyles.h"





SettingsForm::SettingsForm( QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	innerWidget(new QTabWidget(this)),
	generalSettings(new GeneralSettings(this)),
	databaseSettings(new SearchDatabaseSettings(this)),
	viewSettings(new ViewSettings(this)),
	backButton(new MegaIconButton(this))
{
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(backButton);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setFont(AppGenFont);
	innerWidget->addTab(generalSettings, tr("General"));
	innerWidget->addTab(databaseSettings, tr("Database"));
	innerWidget->addTab(viewSettings, tr("View"));
	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	QObject::connect(backButton, &MegaIconButton::clicked, this, &SettingsForm::saveAndExit);
	QObject::connect(generalSettings, &GeneralSettings::retranslated, this, &SettingsForm::retranslation);
}

void SettingsForm::retranslation()
{
	databaseSettings->retranslate();
	innerWidget->setTabText(0, tr("General"));
	innerWidget->setTabText(1, tr("Database"));
	innerWidget->setTabText(2, tr("View"));
	backButton->setText(tr("back"));
	viewSettings->retranslate();
	emit retranslated();
}

void SettingsForm::saveAndExit()
{
	generalSettings->extractAndSave();
	databaseSettings->extractAndSave();
	viewSettings->extractAndSave();
	AppSettings->Save();
	emit backRequired();
	emit fontsChanged();
}








