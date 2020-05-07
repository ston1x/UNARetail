#include "ExtraScaningSettings.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"
ExtraScaningSettings::ExtraScaningSettings(Modes mode, QWidget* parent)
:	QWidget(parent),
mainLayout(new QFormLayout(this)),
floatControl(new MegaIconButton(this)),
allowPrefixScaning(new MegaIconButton(this)),
prefixExtrabarcode(new QLineEdit(this)),
clearScanBuffer(new MegaIconButton(this)),
currentMode(mode)
{
	mainLayout->addRow(tr("float control"), floatControl);
	mainLayout->addRow(tr("Partial barcode search"), allowPrefixScaning);
	mainLayout->addRow(tr("Enter prefix"), prefixExtrabarcode);
	mainLayout->addRow(tr("clear scan buffer"), clearScanBuffer);
	prefixExtrabarcode->setText(AppSettings->extrasearchPrefix);
	allowPrefixScaning->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	allowPrefixScaning->setCheckable(true);
	allowPrefixScaning->setChecked(!AppSettings->extrasearchPrefix.isEmpty());
	clearScanBuffer->setCheckable(true);
	clearScanBuffer->setChecked(AppSettings->clearScanBuffer);
	clearScanBuffer->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	clearScanBuffer->setIcon(QIcon(":/res/data2.png"));
	prefixExtrabarcode->setEnabled(allowPrefixScaning->isChecked());
	floatControl->setCheckable(true);
	floatControl->setChecked(AppSettings->floatControl[int(currentMode)]);
	floatControl->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	floatControl->setIcon(QIcon(":/res/toinput.png"));
#ifdef QT_VERSION5X
	QObject::connect(allowPrefixScaning, &MegaIconButton::toggled, this, &ExtraScaningSettings::prefixScaningPressed);
#endif
}

void ExtraScaningSettings::extractAndSave()
{
	AppSettings->floatControl[int(currentMode)] = floatControl->isChecked();
	AppSettings->extrasearchPrefix = prefixExtrabarcode->text();
	AppSettings->clearScanBuffer = clearScanBuffer->isChecked();
}

void ExtraScaningSettings::prefixScaningPressed(bool check)
{
	if (!check)
	{
		prefixExtrabarcode->clear();
	}
	prefixExtrabarcode->setEnabled(check);
}
