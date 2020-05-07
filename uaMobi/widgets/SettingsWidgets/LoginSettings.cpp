#include "LoginSettings.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"
void LoginSettings::_setLoginPass()
{
	loginField->setEnabled(enableLoginSend->isChecked());
	passwordField->setEnabled(enableLoginSend->isChecked());
}
LoginSettings::LoginSettings(QWidget* parent)
	: QWidget(parent),
	mainLayout(new QFormLayout(this)),
	enableLoginSend(new MegaIconButton(this)),
	loginField(new QLineEdit(this)),
	passwordField(new QLineEdit(this))
{
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	mainLayout->addRow(tr("enable login send"), enableLoginSend);
	mainLayout->addRow(tr("login field"), loginField);
	mainLayout->addRow(tr("password field"), passwordField);

	enableLoginSend->setCheckable(true);
	enableLoginSend->setChecked(AppSettings->sendLogin);
	enableLoginSend->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	enableLoginSend->setIcon(QIcon(":/res/toinput.png"));
	_setLoginPass();
	
	loginField->setText(AppSettings->userLogin);
	loginField->setValidator(new LoginPassValidator(loginField));
	passwordField->setText(AppSettings->userPass);
	passwordField->setValidator(new LoginPassValidator(passwordField));
	passwordField->setEchoMode(QLineEdit::Password);
	
	QObject::connect(enableLoginSend, &MegaIconButton::toggled, this, &LoginSettings::loginEnableToggled);

}

void LoginSettings::extractAndSave()
{
	AppSettings->sendLogin = enableLoginSend->isChecked();
	AppSettings->userLogin = loginField->text();
	AppSettings->userPass = passwordField->text();
}

void LoginSettings::loginEnableToggled()
{
	_setLoginPass();
	if (!enableLoginSend->isChecked())
	{
		loginField->clear();
		passwordField->clear();
	}
}

QValidator::State LoginPassValidator::validate(QString& input, int& pos) const
{
	if (input.count() > 20)
		return QValidator::State::Invalid;
	if (input.contains('\n'))
		return QValidator::State::Invalid;
	return QValidator::State::Acceptable;
}

LoginPassValidator::LoginPassValidator(QObject* parent)
	: QValidator(parent)
{
}
