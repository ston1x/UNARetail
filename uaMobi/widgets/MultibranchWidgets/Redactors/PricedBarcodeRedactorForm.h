#pragma once
#include "Abs_Redactor.h"
#include "widgets/ControlsMiniwidgets/QuantityControl.h"


class PricedBarcodeRedactorForm : public Abs_Redactor
{
	Q_OBJECT
protected:
	QuantityControl* genPrice;
	QuantityControl* discPrice;

	PricedBarcode redacted;
public:
	PricedBarcodeRedactorForm(QWidget* parent);
	virtual bool setRedacted(Entity e) override;
	virtual void extractAndSave() override;

};