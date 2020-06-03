#pragma once
#include <QWidget>
#include "dataproviders/ModesDescriptions.h"

class QFormLayout;
class MegaIconButton;
class QLineEdit;
class TaxInvoiceSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	MegaIconButton* allowTaxInvoice;
	MegaIconButton* requireTaxInvoice;

	Modes currentMode;
public:
	TaxInvoiceSettings(Modes mode, QWidget* parent);

	void extractAndSave();
protected slots:
	void allowPressed();
};




