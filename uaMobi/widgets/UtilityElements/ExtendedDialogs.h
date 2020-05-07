#pragma once
#include <QDialog>
#include <QIcon>

class QFormLayout;
class QLineEdit;
class QHBoxLayout;
class MegaIconButton;
class QVBoxLayout;
class QGridLayout;
class QLabel;
class QToolButton;
class LoginPassDialog : public QDialog
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QToolButton* quitButton;
	QLineEdit* loginField;
	QLineEdit* passField;
	QHBoxLayout* buttonPanel;
	MegaIconButton* okButton;
	MegaIconButton* cancelButton;
public:
	enum LoginStatus { cancelled = 0, confirmed=1};
	LoginPassDialog(QWidget* parent = Q_NULLPTR);

	static int ascLoginPass();
protected slots:
	void okPressed();
	void cancelPressed();
};
class ErrorMessageDialog : public QDialog
{
	Q_OBJECT
protected:
    QVBoxLayout* mainLayout;
    QHBoxLayout* topLayout;
	QLabel* topic;
	QToolButton* quitButton;
    QHBoxLayout* middleLayout;
	QLabel* errorMessage;
	QLabel* errorImage;
    QHBoxLayout* bottomLayout;
	MegaIconButton* okButton;
	MegaIconButton* infoButton;
	QLabel* stackTrace;
public:
	explicit ErrorMessageDialog(const QString header = QString(), const QString text = QString(), const QString stack = QString(), QWidget* parent = Q_NULLPTR,const QIcon icon  = QIcon());
	static void showErrorInfo(const QString& header, const QString& message,
		bool showStack = false,
		const QString stack = QString(),
		const QIcon& errorIcon = QIcon());
protected slots:
	void okPressed();
	void infoToggled();
};
