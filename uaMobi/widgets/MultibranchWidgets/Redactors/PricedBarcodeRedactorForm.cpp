#include "PricedBarcodeRedactorForm.h"

PricedBarcodeRedactorForm::PricedBarcodeRedactorForm(QWidget* parent)
	: Abs_Redactor(parent),
	genPrice(new QuantityControl(
		false,
		tr("General price"),
		this
	)),
	discPrice(new QuantityControl(
		false,
		tr("Discount price"),
		this
	))
{
	mainLayout->addWidget(genPrice->myWidget());
	mainLayout->addWidget(discPrice->myWidget());
	genPrice->setMinimum(1);
	discPrice->setMinimum(1);
#ifdef QT_VERSION5X
	QObject::connect(genPrice, &QuantityControl::editingFinished, discPrice, &QuantityControl::setFocus);
	QObject::connect(discPrice, &QuantityControl::editingFinished, this, &Abs_Redactor::done);
#else
	QObject::connect(genPrice, SIGNAL(editingFinished()), discPrice, SLOT(setFocus()));
	QObject::connect(discPrice, SIGNAL(editingFinished()), this, SLOT(done()));
#endif
}

bool PricedBarcodeRedactorForm::setRedacted(Entity e)
{
	redacted = upcastEntity<PricedBarcodeEntity>(e);
	if (redacted == Q_NULLPTR)
	{
		return false;
	}
	barcodeInfo->setText(redacted->barcode);
	additionalInfo->setText(redacted->addDate.toString(datetimeDBEncoding));
	genPrice->setValue(QString::number(redacted->generalPrice));
	discPrice->setValue(QString::number(redacted->discountPrice));
	return true;
}

void PricedBarcodeRedactorForm::extractAndSave()
{
	redacted->generalPrice = genPrice->getValue().toDouble();
	redacted->discountPrice = discPrice->getValue().toDouble();
}
