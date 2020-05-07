#include "ReceiveWidget.h"
#include <QMessageBox>
#include "widgets/utils/ElementsStyles.h"
#include "widgets/UtilityElements/ExtendedDialogs.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif


bool _verifyLoginPass()
{
	if (AppSettings->userLogin.isEmpty() || AppSettings->userPass.isEmpty())
	{
		return LoginPassDialog::ascLoginPass();
	}
	return true;
}


ReceiveWidget::ReceiveWidget(Modes mode, QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	semaphor(new SemaphorLabel(this)),
	infoLayout(new QHBoxLayout(this)),
	totalQuantity(new QLabel(this)), uniqueBarcodes(new QLabel(this)),
	HttpAddress(new QLabel(this)),
	LocalAddress(new QLabel(this)),
	useHttpButton(new MegaIconButton(this)), useLocalButton(new MegaIconButton(this)),
	backButton(new MegaIconButton(this)),
	http(mode),
	tolocal(mode), awaiting(false), currentMode(mode), timeoutTimer(new QTimer(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(semaphor);
	mainLayout->addLayout(infoLayout);
	infoLayout->addWidget(uniqueBarcodes);
	infoLayout->addWidget(totalQuantity);
	mainLayout->addWidget(useHttpButton);
	mainLayout->addWidget(HttpAddress);
	mainLayout->addWidget(useLocalButton);
	mainLayout->addWidget(LocalAddress);
	mainLayout->addWidget(backButton);


	semaphor->setFixedHeight(calculateAdaptiveButtonHeight(0.1));
	semaphor->setText(tr("Sending state"));

	http.setAddress(AppSettings->httpIn.toString());
	tolocal.addressChanged(AppSettings->localfile);

	mainLayout->setSpacing(0);				//	Spacing removed
	mainLayout->setContentsMargins(0, 0, 0, 0);

    HttpAddress->setText(AppSettings->httpIn.toString());
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

#ifdef QT_VERSION5X
	QObject::connect(useHttpButton, &QPushButton::pressed, this, &ReceiveWidget::httpChosen);
	QObject::connect(useLocalButton, &QPushButton::pressed, this, &ReceiveWidget::localChosen);
	QObject::connect(&http, &toHttp::sendSuccesfull, this, &ReceiveWidget::requestEnd);
	QObject::connect(&http, &toHttp::errorReceived, this, &ReceiveWidget::requestFail);
	QObject::connect(&tolocal, &toLocalMemory::operationDone, this, &ReceiveWidget::requestEnd);
	QObject::connect(&tolocal, &toLocalMemory::addressInvalid, this, &ReceiveWidget::localFail);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ReceiveWidget::backRequired);
	QObject::connect(timeoutTimer, &QTimer::timeout, this, &ReceiveWidget::requestTimeout);
	QObject::connect(this, &ReceiveWidget::sendStateChanged, semaphor, &SemaphorLabel::setState);
#else
	QObject::connect(useHttpButton, SIGNAL(clicked()), this, SLOT(httpChosen()));
	QObject::connect(useLocalButton, SIGNAL(clicked()),this, SLOT(localChosen()));
    QObject::connect(&http, SIGNAL(sendSuccesfull()), this, SLOT(requestEnd()));
	QObject::connect(&http, SIGNAL(errorReceived()), this, SLOT(requestFail()));
	QObject::connect(&tolocal, SIGNAL(operationDone(bool)), this, SLOT(requestEnd()));
	QObject::connect(&tolocal, SIGNAL(addressInvalid()), this, SLOT(addressFail()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	throw;
#endif
	timeoutTimer->setInterval(1200000);
}

void ReceiveWidget::show()
{
    HttpAddress->setText(AppSettings->httpIn.toString());
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
	if (AppSettings->sendLogin)
		if (!_verifyLoginPass())
			return;
	if (!awaiting) {
		awaiting = http.send(mode, AppSettings->sendingFormat);
	}
	timeoutTimer->start();
	emit sendStateChanged(SemaphorLabel::awaiting);
}

void ReceiveWidget::localChosen()
{
	tolocal.addressChanged(AppSettings->localfile); // Updating address
	tolocal.send(mode, AppSettings->sendingFormat);
	timeoutTimer->start();
	emit sendStateChanged(SemaphorLabel::awaiting);
}

void ReceiveWidget::requestEnd(QString data)
{
	timeoutTimer->stop();
	AppData->pushSendingBarcodesToSent(currentMode, modenamesLinker[currentMode]);
	emit sendingSuccess();
	emit sendStateChanged(SemaphorLabel::opsuccess);
	QMessageBox::information(this, tr("send_widget_dialog_title_Success"), tr("send_widget_dialog_text_Operation complete : success") + "\n" + data);

}
void ReceiveWidget::requestFail(QString stack,QString message, int code)
{
	timeoutTimer->stop();	
	emit sendStateChanged(SemaphorLabel::opfail);
	AppData->clearSendingSelector(currentMode, modenamesLinker[currentMode]);
#ifdef DEBUG
	detrace_NETERROR(message, stack);
#endif
	ErrorMessageDialog::showErrorInfo(tr("send error"), message, false, stack);
}

void ReceiveWidget::requestTimeout()
{
	requestFail(tr("timeout"), tr("request timeout"), 408);
}

void ReceiveWidget::localFail(QString msg)
{
	QMessageBox::critical(this, tr("error saving to file"), msg);
}
