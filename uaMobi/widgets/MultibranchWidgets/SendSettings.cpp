#include "SendSettings.h"

SendSettings::SendSettings(Entity prototype, QWidget* parent)
	: inframedWidget(parent), 
	mainLayout(new QVBoxLayout(this)),
	innerWidget(new QTabWidget(this)),
	quickSendSettings(new QuickSendSettings(this)),
	serializationSettings(new SerializationSettings(prototype, this)),
	backButton(new MegaIconButton(this))
{
	mainLayout->addWidget(innerWidget);
	innerWidget->addTab(quickSendSettings, tr("QuickSend"));
	innerWidget->addTab(serializationSettings, tr("Serialization"));
	mainLayout->addWidget(backButton);
	
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);



	QObject::connect(backButton, &MegaIconButton::clicked, this, &SendSettings::saveAndExit);
}

void SendSettings::saveAndExit()
{
	quickSendSettings->extractAndSave();
	serializationSettings->extractAndSave();
	AppSettings->Save();
	emit backRequired();
}
