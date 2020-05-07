#include "ModesDescriptions.h"
#include <QTextStream>

bool hasModifiableSysfeed(Modes m)
{
	switch (m)
	{
	case Modes::Invoices:
	case Modes::Simple:
		return true;
	default:
		return false;
	}
}
const int MODES_TOTAL = 6;
 const QString ModePrefixes[MODES_TOTAL] = {
	QStringLiteral("src"),
	QStringLiteral("inv"),
	QStringLiteral("sup"),
	QStringLiteral("sim"),
	QStringLiteral("pri"),
	QStringLiteral("ord")
};

const QString TableSuffixes[MODES_TOTAL] =
{
	QStringLiteral("scan"),
	QStringLiteral("upld"),
	QStringLiteral("bckp")
};

bool ModeDescription::_deserialize(const QString& str)
{
	QStringList temp = str.split(QChar(30));
	int intBuffer;
	bool ok;
	bool bBuffer;
	switch (temp.count())
	{
	default:
	case (4):
		floatControl = (temp.at(3) == "true");
	case (3):
	{
		serializationOrder.clear();
		QStringList sLevelSplit = temp.at(2).split(",");
		for (int i = 0; i < sLevelSplit.count(); ++i)
		{
			intBuffer = sLevelSplit.at(i).toInt(&ok);
			if (ok)
				serializationOrder << intBuffer;
			else
				return false;
		}
	}
	case (2):
		intBuffer = temp.at(1).toInt(&ok);
		if (ok)
			sysfeed = intBuffer;
		else
			return false;
	case (1):
		intBuffer = temp.at(0).toInt(&ok);
		if (ok)
			mode = modeFromInt(intBuffer);
		else
			return false;
		break;
	case (0):
		return false;
	}
	return false;
}

QString ModeDescription::_serialize() const
{
	QString buffer;
	QTextStream txt(&buffer);
	txt << QString::number(int(mode)) << QChar(30) << QString::number(sysfeed) << QChar(30);
	for (int i = 0; i < serializationOrder.count() - 1; ++i)
	{
		txt << QString::number(serializationOrder.at(i)) << ',';
	}
	if (serializationOrder.count() > 0)
		txt << QString::number(serializationOrder.last());
	txt << QChar(30) << (((floatControl) ? "true" : "false"));
	txt.flush();
	return buffer;
}

ModeDescription::ModeDescription()
	:mode(Modes::Search), sysfeed(-1), serializationOrder(), floatControl(false)
{
}

ModeDescription::ModeDescription(Modes md)
	: mode(md), sysfeed(-1), serializationOrder(), floatControl(false)
{
}

ModeDescription::ModeDescription(QString& serialized)
	: mode(Modes::Search), sysfeed(-1), serializationOrder(), floatControl(false)
{
	deserialize(serialized);
}

ModeDescription::ModeDescription(Modes m, int sf, QList<int>& sO, bool fc)
	: mode(m), sysfeed(sf), serializationOrder(sO), floatControl(fc)
{
}

bool ModeDescription::deserialize(const QString& str)
{
	return _deserialize(str);
}

QString ModeDescription::serialize() const
{
	return _serialize();
}

Modes ModeDescription::getMode() const
{
	return mode;
}

int ModeDescription::getSysfeed() const
{
	return sysfeed;
}

QList<int> ModeDescription::getSerializationOrder() const
{
	return serializationOrder;
}

bool ModeDescription::requiresFloatControl() const
{
	return floatControl;
}

void ModeDescription::setSysfeed(int sf)
{
	sysfeed = sf;
}

void ModeDescription::setSerializationOrder(QList<int>& so)
{
	serializationOrder = so;
}

void ModeDescription::setFloatControl(bool fc)
{
	floatControl = fc;
}
