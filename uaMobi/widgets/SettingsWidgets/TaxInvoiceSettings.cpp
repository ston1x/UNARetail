#include "TaxInvoiceSettings.h"
#include <QFormLayout>
#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"

TaxInvoiceSettings::TaxInvoiceSettings(Modes mode, QWidget* parent)
	: QWidget(parent), mainLayout(new QFormLayout(this)),
	allowTaxInvoice(new MegaIconButton(this)), requireTaxInvoice(new MegaIconButton(this)),
	currentMode(mode)
{
	mainLayout->addRow(tr("allow tax invoice"), allowTaxInvoice);
	mainLayout->addRow(tr("require tax invoice"), requireTaxInvoice);
	allowTaxInvoice->setIcon(QIcon(":/res/barcode.png"));
	requireTaxInvoice->setIcon(QIcon(":/res/url2.png"));
	allowTaxInvoice->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	requireTaxInvoice->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	allowTaxInvoice->setCheckable(true);
	allowTaxInvoice->setChecked(AppSettings->getModeDescription(currentMode).isInsertingTaxInvoiceNumAllowed());
	requireTaxInvoice->setCheckable(true);
	requireTaxInvoice->setChecked(AppSettings->getModeDescription(currentMode).isForbiddenInsertingWithoutTaxInvoice());
	QObject::connect(allowTaxInvoice, &MegaIconButton::clicked, this, &TaxInvoiceSettings::allowPressed);
	allowPressed();
}

void TaxInvoiceSettings::extractAndSave()
{
	AppSettings->getModeDescription(currentMode).setInsertingTaxNumber(allowTaxInvoice->isChecked());
	AppSettings->getModeDescription(currentMode).setForbiddingInsertingWithoutTaxInvoice(
		allowTaxInvoice->isChecked() && requireTaxInvoice->isChecked());
}

void TaxInvoiceSettings::allowPressed()
{
	requireTaxInvoice->setEnabled(allowTaxInvoice->isChecked());
}
