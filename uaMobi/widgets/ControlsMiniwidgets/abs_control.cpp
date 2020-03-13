#include "abs_control.h"
#include "QuantityControl.h"
#include "ControlTranslator.h"
//#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif



abs_control::abs_control(QWidget* parent, InputControlEntity::ControlTypes Type, QString cname)
    : QObject(parent), type(Type), parent(parent), label(makeControlTranslation(cname)),name(cname)
{
#ifdef DEBUG
	detrace_DCONSTR("control-like of type " << type);
#endif
}

QString abs_control::getValue() const
{
	return prepareAndReturnValue();
}

QWidget* abs_control::myWidget() const
{
	return getInnerWidget();
}

InputControlEntity::ControlTypes abs_control::myType()
{
	return type;
}

bool abs_control::setValue(QString val)
{
	return parseAndSetValue(val);
}

void abs_control::reset()
{
	clear();
}

abs_control::~abs_control()
{
}

bool abs_control::canGiveValue() const
{
	return valueAvailable();
}

void abs_control::connectAsNext(abs_control* ctrl)
{
	if (ctrl != Q_NULLPTR)
	{
#ifdef QT_VERSION5X
		QObject::connect(ctrl, &abs_control::editingFinished, this, &abs_control::setFocus);
#else
		QObject::connect(ctrl, SIGNAL(editingFinished()), this, SLOT(setFocus()));
#endif
	}

}

void abs_control::subcontrolEditingFinished()
{
	if (valueAvailable())
	{
		emit editingFinished();
		emit valueReceived(prepareAndReturnValue());
	}
}


QString chopZeros(QString val, bool cdot)
// removes .0000 from end of the line to avoid cursor-to-end behaviour
{
	val.replace(',', '.');
	if (cdot)
	{
		while (val.endsWith('0'))
		{
			val.chop(1);
		}
	}
	else
	{
		while (val.endsWith('0') || val.endsWith('.'))
		{
			val.chop(1);
		}
	}
	return val;
}
