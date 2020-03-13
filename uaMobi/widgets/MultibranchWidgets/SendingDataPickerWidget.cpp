#include "SendingDataPickerWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include <QtWidgets/qmessagebox.h>
#include <QtCore/QTimer>
SendingDataPickerWidget::SendingDataPickerWidget(Modes mode, QWidget* parent)
	:inframedWidget(parent), abstractNode(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	unsentButton(new MegaIconButton(innerWidget)), sentButton(new MegaIconButton(innerWidget)),
	allButton(new MegaIconButton(innerWidget)), sentQuantityInfo(new QLabel(innerWidget)),
	unsentQuantityInfo(new QLabel(innerWidget)), totalQantityInfo(new QLabel(innerWidget)),
	backButton(new MegaIconButton(innerWidget)),
	currentMode(mode)
	, sendWidget(new ReceiveWidget(mode, this)), 
	settingsWidget(new SendSettings(this))
{
	this->setLayout(mainLayout);
	main = this;
	current = untouchable = innerWidget;
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(sentQuantityInfo);
	innerLayout->addWidget(sentButton);
	innerLayout->addWidget(unsentQuantityInfo);
	innerLayout->addWidget(unsentButton);
	innerLayout->addWidget(totalQantityInfo);
	innerLayout->addWidget(allButton);
	innerLayout->addWidget(backButton);
	mainLayout->addWidget(sendWidget);
	mainLayout->addWidget(settingsWidget);

	sendWidget->hide();
	settingsWidget->hide();

	sentButton->setIcon(QIcon(":/res/alreadySent.png"));
	unsentButton->setIcon(QIcon(":/res/viaNet.png"));
	allButton->setIcon(QIcon(":/res/viaFile.png"));
	backButton->setIcon(QIcon(":/res/back.png"));

	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	sentQuantityInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	unsentQuantityInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	totalQantityInfo->setStyleSheet(COUNTERS_LABEL_STYLESHEET);

	sentButton->setText(tr("send_sent_button"));
	unsentButton->setText(tr("send_unsent_button"));
	allButton->setText(tr("send_all_button"));

	QObject::connect(unsentButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::unsentChosen);
	QObject::connect(sentButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::sentChosen);
	QObject::connect(allButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::allChosen);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SendingDataPickerWidget::backRequired);
	QObject::connect(sendWidget, &ReceiveWidget::backRequired, this, &SendingDataPickerWidget::hideCurrent);
	QObject::connect(sendWidget, &ReceiveWidget::sendingSuccess, this, &SendingDataPickerWidget::sendingSuccess);
	QObject::connect(settingsWidget, &SendSettings::backRequired, this, &SendingDataPickerWidget::hideCurrent);
}

void SendingDataPickerWidget::show()
{
	set_info();
	inframedWidget::show();
}

bool SendingDataPickerWidget::back()
{
#ifdef DEBUG
	detrace_METHCALL("SendingDataPickerWidget::back");
#endif

	if (current->back())
		return true;
	else if (current != innerWidget)
	{
#ifdef DEBUG
		detrace_METHEXPL("SendingDataPicker returning true");
#endif

		hideCurrent();
		return true;
	}

#ifdef DEBUG
	detrace_METHEXPL("SendingDataPicker returning false");
#endif

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
	sentQuantityInfo->setText(tr("total_barcodes_quantity\n") +
		QString::number(AppData->countAllIn(currentMode, TableNames::Uploaded)));
	unsentQuantityInfo->setText(tr("total_barcodes_quantity\n") +
			QString::number(AppData->countAllIn(currentMode, TableNames::Scanned)));
	totalQantityInfo->setText(tr("total_barcodes_quantity\n") +
			QString::number(AppData->countAllIn(currentMode, TableNames::Uploaded) +
				AppData->countAllIn(currentMode, TableNames::Scanned)));
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
	emit sendingSuccesfull();
	QTimer::singleShot(1000, this, &SendingDataPickerWidget::set_info);
}
