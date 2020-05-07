#include "SendingDataPickerWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include <QMessageBox>
#include <QtCore/QTimer>


SendingDataPickerWidget::SendingDataPickerWidget(Modes mode, QWidget* parent)
	:inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	semaphor(new SemaphorLabel(this)),
	unsentButton(new MegaIconButton(innerWidget)), sentButton(new MegaIconButton(innerWidget)),
	allButton(new MegaIconButton(innerWidget)), settingsButton(new MegaIconButton(innerWidget)),
	sentQuantityInfo(new CounterLabel(innerWidget)),
	unsentQuantityInfo(new CounterLabel(innerWidget)), 
	totalQantityInfo(new CounterLabel(innerWidget)),
	backButton(new MegaIconButton(innerWidget)),
	currentMode(mode)
	, sendWidget(new ReceiveWidget(mode, this)), 
	settingsWidget(new SendSettings(mode, modenamesLinker[mode], this))
{
	this->setLayout(mainLayout);
	main = this;
	current = untouchable = innerWidget;
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(semaphor);
	innerLayout->addWidget(sentQuantityInfo);
	innerLayout->addWidget(sentButton);
	innerLayout->addWidget(unsentQuantityInfo);
	innerLayout->addWidget(unsentButton);
	innerLayout->addWidget(totalQantityInfo);
	innerLayout->addWidget(allButton);
	innerLayout->addWidget(settingsButton);
	innerLayout->addWidget(backButton);
	mainLayout->addWidget(sendWidget);
	mainLayout->addWidget(settingsWidget);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);


	semaphor->setFixedHeight(calculateAdaptiveButtonHeight(0.1));
	semaphor->setText(tr("Sending state"));
	
	sendWidget->hide();
	settingsWidget->hide();

	sentButton->setIcon(QIcon(":/res/alreadySent.png"));
	unsentButton->setIcon(QIcon(":/res/viaNet.png"));
	allButton->setIcon(QIcon(":/res/viaFile.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	settingsButton->setIcon(QIcon(":/res/settings.png"));

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	if (hasModifiableSysfeed(mode))
	{
		settingsWidget->showExtraSettings();
	}

	sentButton->setText(tr("send_sent_button"));
	unsentButton->setText(tr("send_unsent_button"));
	allButton->setText(tr("send_all_button"));
	settingsButton->setText(tr("Settings"));
#ifdef QT_VERSION5X
	QObject::connect(unsentButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::unsentChosen);
	QObject::connect(sentButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::sentChosen);
	QObject::connect(allButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::allChosen);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::backRequired);
	QObject::connect(sendWidget, &ReceiveWidget::backRequired, this, &SendingDataPickerWidget::hideCurrent);
	QObject::connect(sendWidget, &ReceiveWidget::sendingSuccess, this, &SendingDataPickerWidget::sendingSuccess);
	QObject::connect(settingsWidget, &SendSettings::backRequired, this, &SendingDataPickerWidget::hideCurrent);
	QObject::connect(settingsButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::showSettings);
	QObject::connect(sendWidget, &ReceiveWidget::sendStateChanged, semaphor, &SemaphorLabel::setState);
#else
	QObject::connect(unsentButton, SIGNAL(clicked()), this, SLOT(unsentChosen()));
	QObject::connect(sentButton, SIGNAL(clicked()), this, SLOT(sentChosen()));
	QObject::connect(allButton, SIGNAL(clicked()), this, SLOT(allChosen()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(sendWidget, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(sendWidget, SIGNAL(sendingSuccess()), this, SLOT(sendingSuccess()));
	QObject::connect(settingsWidget, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(showSettings()));
	QObject::connect(sendWidget, SIGNAL(sendStateChanged(int)), semaphor, SLOT(setState(int)));
#endif
}

void SendingDataPickerWidget::show()
{
	set_info();
	inframedWidget::show();
}

bool SendingDataPickerWidget::back()
{

	if (current->back())
		return true;
	else if (current != innerWidget)
    {
		hideCurrent();
		return true;
	}


	return false;
}

bool SendingDataPickerWidget::isExpectingControl(int value)
{
	if (sendWidget->isExpectingControl(value))
	{
		return true;
	}
	if (innerWidget->isHidden())
	{
		return false;
	}
	switch (value)
	{
	case 0:
		unsentChosen();
		break;
	case 1:
		sentChosen();
		break;
	case 2:
		allChosen();
		break;
	case 3:
		emit backRequired();
		break;
	default:
		break;
	}
	return true;
}

bool SendingDataPickerWidget::giveSettings()
{
	if (current->giveSettings())
		return true;
	if (current == innerWidget)
	{
		_hideAny(settingsWidget);
	}
	else
	{
		hideCurrent();
	}
	return true;
}

void SendingDataPickerWidget::set_info()
{
	sentQuantityInfo->setText(tr("total_barcodes_quantity\n"));
	sentQuantityInfo->setCounter(AppData->countAllIn(currentMode, TableNames::Uploaded));
	unsentQuantityInfo->setText(tr("total_barcodes_quantity\n"));
	unsentQuantityInfo->setCounter(AppData->countAllIn(currentMode, TableNames::Scanned));
	totalQantityInfo->setText(tr("total_barcodes_quantity\n") );
	totalQantityInfo->setCounter(AppData->countAllIn(currentMode, TableNames::Uploaded) +
		AppData->countAllIn(currentMode, TableNames::Scanned));
}

void SendingDataPickerWidget::unsentChosen()
{
	
	sendWidget->setMode(sendUnsent);
	if (AppSettings->simpleSending)
	{
		sendWidget->sendBySetup();
		return;
	}
	innerWidget->hide();
	current = sendWidget;
	current->show();
}

void SendingDataPickerWidget::sentChosen()
{
	sendWidget->setMode(sendSent);
	if (AppSettings->simpleSending)
	{
		sendWidget->sendBySetup();
		return;
	}
	_hideAny(sendWidget);
}

void SendingDataPickerWidget::allChosen()
{
	sendWidget->setMode(sendAll);
	if (AppSettings->simpleSending)
	{
		sendWidget->sendBySetup();
		return;
	}
	_hideAny(sendWidget);
}

void SendingDataPickerWidget::hideCurrent()
{
	_hideCurrent(untouchable);
	set_info();
}

void SendingDataPickerWidget::sendingSuccess()
{

    QTimer::singleShot(500, this, SLOT(set_info()));
}

void SendingDataPickerWidget::showSettings()
{
	_hideAny(settingsWidget);
}
