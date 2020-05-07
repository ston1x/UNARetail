#include "ExtendedDialogs.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QStyle>
#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"

LoginPassDialog::LoginPassDialog(QWidget* parent)
	:QDialog(parent), mainLayout(new QFormLayout(this)),
	loginField(new QLineEdit(this)),
	passField(new QLineEdit(this)),
	buttonPanel(new QHBoxLayout(this)),
	okButton(new MegaIconButton(this)),
	cancelButton(new MegaIconButton(this))
{
	setFont(AppGenFont);
	mainLayout->addRow("Login field", loginField);
	mainLayout->addRow("password field", passField);
	mainLayout->addItem(buttonPanel);
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonPanel->addWidget(cancelButton);
	buttonPanel->addWidget(okButton);
	buttonPanel->setSpacing(0);
	buttonPanel->setContentsMargins(0, 0, 0, 0);
	loginField->setText(AppSettings->userLogin);
	passField->setText(AppSettings->userPass);
	passField->setEchoMode(QLineEdit::Password);
	okButton->setIcon(QIcon(":/res/forward.png"));
	okButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	cancelButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	cancelButton->setIcon(QIcon(":/res/back.png"));
	setStyleSheet("QDialog {border: 2px solid black;}");
#ifdef Q_OS_ANDROID
	setFixedWidth(calculateAdaptiveWidth(0.9));
#endif
	QObject::connect(okButton, &MegaIconButton::clicked, this, &LoginPassDialog::okPressed);
	QObject::connect(cancelButton, &MegaIconButton::clicked, this, &LoginPassDialog::cancelPressed);
	QObject::connect(loginField, &QLineEdit::returnPressed, passField, QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(passField, &QLineEdit::returnPressed, this, &LoginPassDialog::okPressed);


}

int LoginPassDialog::ascLoginPass()
{
	int temp;
	LoginPassDialog* lpd = new LoginPassDialog;
	temp = lpd->exec();
	lpd->deleteLater();
	return temp;
}

void LoginPassDialog::okPressed()
{
	if (!(loginField->text().isEmpty()) && (loginField->text().count() < 20))
	{
		if (!(passField->text().isEmpty()) && (passField->text().count() < 20))
		{
			AppSettings->userLogin = loginField->text();
			AppSettings->userPass = passField->text();
			emit done(confirmed);
		}
	}
}

void LoginPassDialog::cancelPressed()
{
	emit done(cancelled);
}

void ErrorMessageDialog::okPressed()
{
    emit done(0);
}

ErrorMessageDialog::ErrorMessageDialog(const QString header,
	const QString text, const QString stack, QWidget* parent, const QIcon icon)
    :QDialog(parent), mainLayout(new QVBoxLayout(this)),
      topLayout(new QHBoxLayout(this)),
	topic(new QLabel(this)),
	quitButton(new QToolButton(this)),
    middleLayout(new QHBoxLayout(this)),
	errorMessage(new QLabel(this)),
	errorImage(new QLabel(this)),
    bottomLayout(new QHBoxLayout(this)),
	okButton(new MegaIconButton(this)),
	infoButton(new MegaIconButton(this)),
	stackTrace(new QLabel(this))
{
	setFixedWidth(calculateAdaptiveWidth(0.9));
	setWindowTitle(header);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(middleLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addWidget(stackTrace);
    topLayout->addWidget(topic);
    topLayout->addWidget(quitButton);
    middleLayout->addWidget(errorImage);
    middleLayout->addWidget(errorMessage);
    bottomLayout->addWidget(okButton);
    bottomLayout->addWidget(infoButton);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    topLayout->setSpacing(0);
    topLayout->setContentsMargins(0,0,0,0);

    middleLayout->setSpacing(0);
    middleLayout->setContentsMargins(0,0,0,0);

    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(0,0,0,0);
    setStyleSheet("QDialog {border: 2px solid black;}");
    setFont(AppGenFont);
	topic->setText(header);
	quitButton->setIcon(style()->standardIcon(QStyle::StandardPixmap::SP_DialogCloseButton));
    quitButton->setFixedSize(calculateAdaptiveSize(0.06, 0.2));

	errorMessage->setText(text);
	if (icon.isNull())
	{
        errorImage->setPixmap(style()->standardIcon(
            QStyle::StandardPixmap::SP_MessageBoxCritical
        ).pixmap(calculateAdaptiveSize(0.1, 0.3)));
	}
	else
	{
        errorImage->setPixmap(icon.pixmap(calculateAdaptiveSize(0.1, 0.3)));
	}
    errorImage->setFixedHeight(calculateAdaptiveButtonHeight(0.1));
	okButton->setIcon(QIcon(":/res/submit.png"));
	okButton->setText(tr("OK"));
    okButton->setFixedHeight(calculateAdaptiveButtonHeight(0.06));
    infoButton->setFixedHeight(calculateAdaptiveButtonHeight(0.06));
	infoButton->setIcon(QIcon(":/downarrow.png"));
	infoButton->setText(tr("info"));
	stackTrace->setText(stack);
    stackTrace->setWordWrap(true);
	stackTrace->hide();
	infoButton->setCheckable(true);
	infoButton->setChecked(false);
	infoButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	QObject::connect(infoButton, &MegaIconButton::clicked, this, &ErrorMessageDialog::infoToggled);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &ErrorMessageDialog::okPressed);
	QObject::connect(quitButton, &MegaIconButton::clicked, this, &ErrorMessageDialog::okPressed);
}

void ErrorMessageDialog::showErrorInfo(const QString& header, const QString& message, bool showStack, const QString stack, const QIcon& errorIcon)
{
	int temp;
	ErrorMessageDialog* lpd = new ErrorMessageDialog(header, message, stack, Q_NULLPTR, errorIcon);
	if (showStack)
	{
		lpd->infoToggled();
	}
	temp = lpd->exec();
	lpd->deleteLater();
}

void ErrorMessageDialog::infoToggled()
{
	if (infoButton->isChecked())
	{
		stackTrace->show();
	}
	else
	{
		stackTrace->hide();
	}
}
