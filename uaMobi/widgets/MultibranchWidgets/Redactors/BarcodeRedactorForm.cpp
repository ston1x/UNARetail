#include "BarcodeRedactorForm.h"

BarcodeRedactorForm::BarcodeRedactorForm(QWidget* parent)
	:Abs_Redactor(parent), quantityEdit(new QuantityControl(true,
		tr("Quantity"),
		this))
{
	mainLayout->addWidget(quantityEdit->myWidget());
	quantityEdit->setMinimum(1);
	quantityEdit->show();
#ifdef QT_VERSION5X
	QObject::connect(quantityEdit, &QuantityControl::editingFinished, this, &Abs_Redactor::done);
#else
    QObject::connect(quantityEdit, SIGNAL(editingFinished()), this, SIGNAL(done()));
#endif
}

bool BarcodeRedactorForm::setRedacted(Entity e)
{
	redacted = upcastEntity<BarcodeEntity>(e);
	if (redacted == Q_NULLPTR)
	{
		return false;
	}
	barcodeInfo->setText(redacted->barcode);
	additionalInfo->setText(redacted->addDate.toString(datetimeDBEncoding));
	quantityEdit->setValue(QString::number(redacted->quantity));
	return true;
}

void BarcodeRedactorForm::extractAndSave()
{
	redacted->quantity = quantityEdit->getValue().toInt();
}
