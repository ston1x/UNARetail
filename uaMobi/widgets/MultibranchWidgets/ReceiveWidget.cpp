#include "ReceiveWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include <QtWidgets/qmessagebox.h>
#ifdef DEBUG
#include "debugtrace.h"
#endif



ReceiveWidget::ReceiveWidget(Modes mode, QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	infoLayout(new QHBoxLayout(this)),
	totalQuantity(new QLabel(this)), uniqueBarcodes(new QLabel(this)),
	HttpAddress(new QLabel(this)),
	LocalAddress(new QLabel(this)),
	useHttpButton(new MegaIconButton(this)), useLocalButton(new MegaIconButton(this)),
	backButton(new MegaIconButton(this)),
	http(mode),
	tolocal(mode), awaiting(false), currentMode(mode)
{
	this->setLayout(mainLayout);
	mainLayout->addLayout(infoLayout);
	infoLayout->addWidget(uniqueBarcodes);
	infoLayout->addWidget(totalQuantity);
	mainLayout->addWidget(useHttpButton);
	mainLayout->addWidget(HttpAddress);
	mainLayout->addWidget(useLocalButton);
	mainLayout->addWidget(LocalAddress);
	mainLayout->addWidget(backButton);

	http.addressChanged(AppSettings->httpIn.toString(), true);
	http.addressChanged(AppSettings->httpOut.toString(), false);
	tolocal.addressChanged(AppSettings->localfile, false);

	mainLayout->setSpacing(0);				//	Spacing removed
	mainLayout->setContentsMargins(0, 0, 0, 0);

	HttpAddress->setText(AppSettings->httpOut.toDisplayString());
	LocalAddress->setText(AppSettings->localfile);
	useHttpButton->setText(tr("send_widget_via HTTP"));
	useLocalButton->setText(tr("send_widget_on local"));
	backButton->setText(tr("branch_widget_back"));
	useHttpButton->setIcon(QIcon(":/res/viaNet.png"));
	useLocalButton->setIcon(QIcon(":/res/viaFile.png"));
	backButton->setIcon(QIcon(":/res/back.png"));

	QSizePolicy sp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	useHttpButton->setSizePolicy(sp);
	useLocalButton->setSizePolicy(sp);

	totalQuantity->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	uniqueBarcodes->setStyleSheet(COUNTERS_LABEL_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	QObject::connect(useHttpButton, &QPushButton::pressed, this, &ReceiveWidget::httpChosen);
	QObject::connect(useLocalButton, &QPushButton::pressed, this, &ReceiveWidget::localChosen);
	QObject::connect(&http, &toHttp::operationDone, this, &ReceiveWidget::requestEnd);
	QObject::connect(&tolocal, &toLocalMemory::operationDone, this, &ReceiveWidget::requestEnd);
	QObject::connect(&http, &toHttp::addressInvalid, this, &ReceiveWidget::addressFail);
	QObject::connect(&tolocal, &toLocalMemory::addressInvalid, this, &ReceiveWidget::addressFail);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ReceiveWidget::backRequired);
}

void ReceiveWidget::show()
{
#ifdef DEBUG
	detrace_METHCALL("ReceiveWidget::show");
	detrace_METHDATAS("ReceiveWidget::show", " globalAppSettings: http, tcp, local ", << AppSettings->httpOut.toString() << AppSettings->localfile);
#endif
	HttpAddress->setText(AppSettings->httpOut.toDisplayString());
	LocalAddress->setText(AppSettings->localfile);
	show_info();

	inframedWidget::show();
}

bool ReceiveWidget::isExpectingControl(int value)
{
	if (this->isHidden())
		return false;
	switch (value)
	{
	case 0:
		httpChosen();
		break;
	case 1:
		localChosen();
		break;
	case 2:
		emit backRequired();
		break;
	default:
		break;
	}
	return true;
}

bool ReceiveWidget::giveSettings()
{
	return false;
}

void ReceiveWidget::setMode(sendingMode md)
{
	mode = md;
}

void ReceiveWidget::show_info()
{
	switch (mode)
	{
	case sendSent:
		totalQuantity->setText(tr("total_barcodes_quantity\n") + 
			QString::number(AppData->countAllIn(currentMode, TableNames::Uploaded) ));
		uniqueBarcodes->setText(tr("unique_barcodes_quantity\n") +
			QString::number(AppData->countUniqueIn(currentMode, TableNames::Uploaded)));
		break;
	case sendUnsent:
		totalQuantity->setText(tr("total_barcodes_quantity\n") +
			QString::number(AppData->countAllIn(currentMode, TableNames::Scanned)));
		uniqueBarcodes->setText(tr("unique_barcodes_quantity\n") + 
			QString::number(AppData->countUniqueIn(currentMode, TableNames::Scanned)));
		break;
	case sendAll:
		totalQuantity->setText(tr("total_barcodes_quantity\n") + 
			QString::number(AppData->countAllIn(currentMode, TableNames::Uploaded) +
				AppData->countAllIn(currentMode, TableNames::Scanned)));
		uniqueBarcodes->setText(tr("unique_barcodes_quantity\n") + 
			QString::number(AppData->countUniqueIn(currentMode, TableNames::Uploaded) +
				AppData->countUniqueIn(currentMode, TableNames::Scanned)));
		break;
	default:
		break;
	}
}

bool ReceiveWidget::sendBySetup()
{
	switch (AppSettings->sendingDirection)
	{
	case 0:
		httpChosen();
		return true;
	case 1:
		localChosen();
		return true;
	default:
		return false;
	}
	return true;
}

void ReceiveWidget::httpChosen()
{
	if (!awaiting) {
		awaiting = http.send(mode, AppSettings->sendingFormat);
	}
}

void ReceiveWidget::localChosen()
{
	tolocal.addressChanged(AppSettings->localfile, false); // Updating address
	tolocal.send(mode, AppSettings->sendingFormat);
}

void ReceiveWidget::requestEnd(bool ok)
{
	if (ok)
	{
		QMessageBox::information(this, tr("send_widget_dialog_title_Success"), tr("send_widget_dialog_text_Operation complete : success"));
		AppData->pushSendingBarcodesToSent(currentMode, modenamesLinker[currentMode]);
		emit sendingSuccess();
	}
	else
	{
		QMessageBox::critical(this, tr("send_widget_dialog_title_Fail"), tr("send_widget_dialog_text_Operation incomplete : server unreachable"));
		AppData->clearSendingSelector(currentMode, modenamesLinker[currentMode]);
	}
	setFocus();
	awaiting = false;
}
void ReceiveWidget::addressFail()
{
	QMessageBox::critical(this, tr("send_widget_critical_Fail"), tr("send_widget_Address invalid"));
	awaiting = false;
}