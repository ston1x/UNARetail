#include "SendSettings.h"

SendSettings::SendSettings(Modes cmode, Entity prototype, QWidget* parent)
	: inframedWidget(parent), 
	mainLayout(new QVBoxLayout(this)),
	innerWidget(new QTabWidget(this)),
	quickSendSettings(new QuickSendSettings(cmode,this)),
	serializationSettings(new SerializationSettings(cmode,prototype, this)),
	loginSettings(new LoginSettings(this)),
	extraSettings(Q_NULLPTR),
	backButton(new MegaIconButton(this)),
	currentMode(cmode)
{
	mainLayout->addWidget(innerWidget);

	innerWidget->addTab(quickSendSettings, tr("QuickSend"));
	innerWidget->addTab(serializationSettings, tr("Serialization"));
	innerWidget->addTab(loginSettings, tr("Login"));
	mainLayout->addWidget(backButton);
	
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);


#ifdef QT_VERSION5X
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SendSettings::saveAndExit);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(saveAndExit()));
#endif
}

void SendSettings::showExtraSettings()
{
	extraSettings = new ExtraSendSettings(currentMode,this);
	innerWidget->addTab(extraSettings, tr("Extra"));

}

void SendSettings::saveAndExit()
{
	quickSendSettings->extractAndSave();
	serializationSettings->extractAndSave();
	loginSettings->extractAndSave();
	if (extraSettings != Q_NULLPTR)
		extraSettings->extractAndSave();
	AppSettings->Save();
	emit backRequired();
}
