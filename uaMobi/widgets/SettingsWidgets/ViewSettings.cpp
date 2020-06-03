#include "ViewSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"



ViewSettings::ViewSettings(QWidget* parent)
	: QWidget(parent), mainLayout(new QFormLayout(this)),
	example(new QLabel(tr("Font example"), this)),
	fontMin(new QSpinBox(this)),
	fontMax(new QSpinBox(this)),
	fontPercent(new QSpinBox(this))
{
	mainLayout->addRow(tr("Example:"), example);
	mainLayout->addRow(tr("Minimum:"), fontMin);
	mainLayout->addRow(tr("Maximum:"), fontMax);
	mainLayout->addRow(tr("Percent:"), fontPercent);
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	example->setFont(AppGenFont);
	fontMin->setMinimum(5);
	fontMin->setMaximum(calculateAdaptiveButtonHeight(0.03));
	fontMin->setValue(AppFonts->getMinFontHeight());
	fontMax->setMinimum(15);
	fontMax->setMaximum(calculateAdaptiveButtonHeight(0.08));
	fontMax->setValue(AppFonts->getMaxFontHeight());
	fontPercent->setMinimum(1);
	fontPercent->setMaximum(100);
	fontPercent->setValue(int(AppFonts->getFontPercent() * 100));
#ifdef QT_VERSION5X
	QObject::connect(fontMax, QOverload<int>::of(&QSpinBox::valueChanged), this, &ViewSettings::refreshExample);
	QObject::connect(fontMin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ViewSettings::refreshExample);
	QObject::connect(fontPercent, QOverload<int>::of(&QSpinBox::valueChanged), this, &ViewSettings::refreshExample);
#else
    QObject::connect(fontMax, SIGNAL(valueChanged(int)), this, SLOT(refreshExample()));
    QObject::connect(fontMin, SIGNAL(valueChanged(int)), this, SLOT(refreshExample()));
    QObject::connect(fontPercent,SIGNAL(valueChanged(int)), this, SLOT(refreshExample()));
#endif
}

void ViewSettings::extractAndSave()
{
	AppFonts->reset(fontMin->value(), fontMax->value(), double(fontPercent->value()) / 100.0);
	AppSettings->fontMinHeight = fontMin->value();
	AppSettings->fontMaxHeight = fontMax->value();
	AppSettings->fontPercent = double(fontPercent->value()) / 100;
}

void ViewSettings::retranslate()
{
	example->setText(tr("Font example"));
	qobject_cast<QLabel*>(mainLayout->labelForField(example))->setText(tr("Example:"));
	qobject_cast<QLabel*>(mainLayout->labelForField(fontMin))->setText(tr("Minimum:"));
	qobject_cast<QLabel*>(mainLayout->labelForField(fontMax))->setText(tr("Maximum:"));
	qobject_cast<QLabel*>(mainLayout->labelForField(fontPercent))->setText(tr("Percent:"));
}

void ViewSettings::refreshExample()
{
	QFont adapted = FontAdapter::makeIndependentFont(fontMin->value(), fontMax->value(), double(fontPercent->value()) / 100.0);
	example->setFont(adapted);
}
