#include "ControlTranslator.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif // DEBUG

ControlTranslator::ControlTranslator()
	: QObject(Q_NULLPTR)
{
}

QString ControlTranslator::translate(QString& str)
// This object simply maps normal strings to translations
{
#ifdef DEBUG
	detrace_METHCALL("translate control " << str << str.count() << " |");
#endif
	switch (str.count())
	{
	case 3:
		if (str == "qty")
		{
#ifdef DEBUG
			detrace_METHEXPL("returning" << tr("control_quantity"));
#endif
			return tr("control_quantity");
		}
		break;
	case 6:
		if (str =="addqty")
		{
			return tr("control_addqty");
		}
		break;
	default:
		break;
	}
	return str;
}



static ControlTranslator translator;

QString makeControlTranslation(QString& str)
{
	return translator.translate(str);
}
