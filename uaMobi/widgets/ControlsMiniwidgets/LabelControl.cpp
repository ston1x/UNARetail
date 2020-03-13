#include "LabelControl.h"
#include "widgets/utils/ElementsStyles.h"


using namespace specwidgets;

QString LabelControl::prepareAndReturnValue() const
{
	return innerLabel->text();
}

bool LabelControl::parseAndSetValue(QString txt)
{
	innerLabel->setText(txt);
	return true;
}

void LabelControl::clear()
{
	innerLabel->clear();
}

bool LabelControl::valueAvailable() const
{
	return true;
}

QWidget* LabelControl::getInnerWidget() const
{
	return innerLabel;
}

LabelControl::LabelControl(QString str, QWidget* Parent)
    : abs_control(Parent, InputControlEntity::Label, str),
	innerLabel(new unfocusableLabel(str, parent))
{
	innerLabel->setAlignment(Qt::AlignCenter);
	innerLabel->setMinimumHeight(calculateAdaptiveButtonHeight(0.1));
	innerLabel->setFont(FontAdapter::makeFont(1.3));
#ifdef QT_VERSION5X
	QObject::connect(innerLabel, &unfocusableLabel::returnPressed, this, &LabelControl::subcontrolEditingFinished);
#else
	QObject::connect(innerLabel, SIGNAL(returnPressed()), this, SLOT(subcontrolEditingFinished()));
#endif
	hide();
}

LabelControl::~LabelControl()
{
	innerLabel->blockSignals(true);
	innerLabel->deleteLater();
}

void LabelControl::setFocus() const
{
	innerLabel->setFocus();
}

void LabelControl::show()
{
	innerLabel->show();
}

void LabelControl::hide()
{
	innerLabel->hide();
}
