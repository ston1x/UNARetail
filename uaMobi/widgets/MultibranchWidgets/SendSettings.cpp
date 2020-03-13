#include "SendSettings.h"

SendSettings::SendSettings( QWidget* parent)
	: inframedWidget(parent),
	mainLayout(new QVBoxLayout(this)), aboutWidget(new QLabel(this)),
	sendingProtocolInfo(new QLabel(this)), protocolPicker(new QComboBox(this)),
	sendingFormatInfo(new QLabel(this)), sendingFormatPicker(new QComboBox(this)),
	simpleSendingButton(new MegaIconButton(this)), backButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(aboutWidget);
	mainLayout->addWidget(sendingProtocolInfo);
	mainLayout->addWidget(protocolPicker);
	mainLayout->addWidget(sendingFormatInfo);
	mainLayout->addWidget(sendingFormatPicker);
	mainLayout->addWidget(simpleSendingButton);
	mainLayout->addStretch(0);
	mainLayout->addWidget(backButton);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	aboutWidget->setText(tr("send_settings_about"));
	aboutWidget->setAlignment(Qt::AlignCenter);
	sendingProtocolInfo->setText(tr("send_settings_about_protocol"));
	sendingProtocolInfo->setAlignment(Qt::AlignCenter);
	sendingFormatInfo->setText(tr("send_settings_about_format"));
	sendingFormatInfo->setAlignment(Qt::AlignCenter);

	protocolPicker->addItems(QStringList{ "Http", "Local" });
	protocolPicker->setCurrentIndex((AppSettings->sendingDirection >= 0 && AppSettings->sendingDirection < 2) ? AppSettings->sendingDirection : 0);
	protocolPicker->setEditable(false);

	sendingFormatPicker->addItems(QStringList{ "Xml", "json", "csv" });
	sendingFormatPicker->setCurrentIndex((AppSettings->sendingFormat >= 0 && AppSettings->sendingFormat < 3) ? AppSettings->sendingFormat : 0);
	sendingFormatPicker->setEditable(false);

	simpleSendingButton->setIcon(QIcon(":/res/icn-cloud-blocked-512.png"));
	simpleSendingButton->setText(tr("send_settings_simple_sending"));
	simpleSendingButton->setCheckable(true);
	simpleSendingButton->setChecked(AppSettings->simpleSending);
	simpleSendingButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);



	QObject::connect(simpleSendingButton, &MegaIconButton::clicked, this, &SendSettings::simpleSendingToggled);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SendSettings::backRequired);
	QObject::connect(protocolPicker, QOverload<int>::of(&QComboBox::activated), this, &SendSettings::protocolChanged);
	QObject::connect(sendingFormatPicker, QOverload<int>::of(&QComboBox::activated), this, &SendSettings::formatChanged);
}

bool SendSettings::giveSettings()
{
	return false;
}

void SendSettings::show()
{
	simpleSendingButton->setChecked(AppSettings->simpleSending);
	protocolPicker->setCurrentIndex((AppSettings->sendingDirection >= 0 && AppSettings->sendingDirection < 2) ? AppSettings->sendingDirection : 0);
	sendingFormatPicker->setCurrentIndex((AppSettings->sendingFormat >= 0 && AppSettings->sendingFormat < 3) ? AppSettings->sendingFormat : 0);
	inframedWidget::show();
}

void SendSettings::formatChanged(int index)
{
	AppSettings->sendingFormat = index;
}

void SendSettings::protocolChanged(int index)
{
	AppSettings->sendingDirection = index;
}

void SendSettings::simpleSendingToggled()
{
	AppSettings->simpleSending = simpleSendingButton->isChecked();
}
