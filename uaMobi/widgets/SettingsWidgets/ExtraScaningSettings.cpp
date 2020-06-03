#include "ExtraScaningSettings.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include <QBoxLayout>
#include "widgets/utils/MegaIconButton.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>

ExtraScaningSettings::ExtraScaningSettings(Modes mode, QWidget* parent)
:	QWidget(parent),
mainLayout(new QFormLayout(this)),
floatControl(new MegaIconButton(this)),
allowPrefixScaning(new MegaIconButton(this)),
prefixExtrabarcode(new QLineEdit(this)),
clearScanBuffer(new MegaIconButton(this)),
autoFillQuantity(new MegaIconButton(this)),
currentMode(mode)
{
	mainLayout->addRow(tr("float control"), floatControl);
	mainLayout->addRow(tr("Partial barcode search"), allowPrefixScaning);
	mainLayout->addRow(tr("Enter prefix"), prefixExtrabarcode);
	mainLayout->addRow(tr("clear scan buffer"), clearScanBuffer);
	mainLayout->addRow(tr("Auto fill"), autoFillQuantity);
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
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
	floatControl->setChecked(AppSettings->getModeDescription(currentMode).requiresFloatControl());
	floatControl->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	floatControl->setIcon(QIcon(":/res/toinput.png"));
	autoFillQuantity->setCheckable(true);
	autoFillQuantity->setChecked(AppSettings->autoFillQuantity);
	autoFillQuantity->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	autoFillQuantity->setIcon(QIcon(":/res/plus.png"));
#ifdef QT_VERSION5X
	QObject::connect(allowPrefixScaning, &MegaIconButton::toggled, this, &ExtraScaningSettings::prefixScaningPressed);
#endif
}

void ExtraScaningSettings::extractAndSave()
{
	AppSettings->getModeDescription(currentMode).setFloatControl(floatControl->isChecked());
	AppSettings->extrasearchPrefix = prefixExtrabarcode->text();
	AppSettings->clearScanBuffer = clearScanBuffer->isChecked();
	AppSettings->autoFillQuantity = autoFillQuantity->isChecked();
}

void ExtraScaningSettings::prefixScaningPressed(bool check)
{
	if (!check)
	{
		prefixExtrabarcode->clear();
	}
	prefixExtrabarcode->setEnabled(check);
}
