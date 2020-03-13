#include "BarcodeControl.h"
#include "widgets/utils/ElementsStyles.h"
BarcodeControl::BarcodeControl(QString Name, QWidget* Parent)
    : abs_control(Parent, InputControlEntity::Barcode, Name),
	innerLineEdit(new QLineEdit(parent)), requiresDeactivation(false)
{
	if (!BarcodeObs->activated())
	{
		BarcodeObs->activate();
		requiresDeactivation = true;
	}
	innerLineEdit->setStyleSheet(FOCUSED_LINEEDIT_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(innerLineEdit, &QLineEdit::returnPressed, this, &BarcodeControl::proxyEditingFinished);
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, innerLineEdit, &QLineEdit::setText);
#else
	QObject::connect(innerLineEdit, SIGNAL(returnPressed()), this, SLOT(proxyEditingFinished()));
	QObject::connect(BarcodeObs, SIGNAL(barcodeCaught()), innerLineEdit, SLOT(setText()));
#endif

	hide();
}

BarcodeControl::~BarcodeControl()
{
	innerLineEdit->deleteLater();
}

QString BarcodeControl::prepareAndReturnValue() const
{
	return innerLineEdit->text();
}

bool BarcodeControl::parseAndSetValue(QString str)
{
	innerLineEdit->setText(str);
	return true;
}

void BarcodeControl::clear()
{
	innerLineEdit->clear();
}

bool BarcodeControl::valueAvailable() const
{
	return innerLineEdit->text().isEmpty();
}

QWidget* BarcodeControl::getInnerWidget() const
{
	return innerLineEdit;
}

void BarcodeControl::setFocus() const
{
	innerLineEdit->setFocus();
}

void BarcodeControl::show()
{
	innerLineEdit->show();
}

void BarcodeControl::hide()
{
	innerLineEdit->hide();
}

void BarcodeControl::proxyEditingFinished()
{
	if (requiresDeactivation)
		BarcodeObs->deactivate();
	emit editingFinished();
}
