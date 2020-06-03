#include "ModesDescriptions.h"
#include <QTextStream>
#include "Datacore/PricedBarcodeEntity.h"
#include "Datacore/BarcodeEntity.h"
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
	case 9:
		newBCMustHaveTaxInvoiceNumber = temp.at(8).startsWith("t");
	case 8:
		allowInsertingTaxInvoiceNumber = temp.at(7).startsWith("t");
	case 7:
		clearBeforeAttachingNewData = temp.at(6).startsWith("t");
	case 6:
		attachNewDataToPrevious = (temp.at(5).startsWith("t"));
	case 5:
		previousDocument = temp.at(4).toLongLong(&ok);
		if (!ok) previousDocument = 0;
	case (4):
		floatControl = (temp.at(3) == "true");
	case (3):
	{
		serializationOrder.clear();
		QStringList sLevelSplit = temp.at(2).split(",");
		if (!sLevelSplit.isEmpty())
		{
			for (int i = 0; i < sLevelSplit.count(); ++i)
			{
				intBuffer = sLevelSplit.at(i).toInt(&ok);
				if (ok)
					serializationOrder << intBuffer;
				else
					break;
			}
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
	return true;
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
	txt << QChar(30) << QString::number(previousDocument);
	txt << QChar(30) << ((attachNewDataToPrevious) ? "true" : "false");
	txt << QChar(30) << ((clearBeforeAttachingNewData) ? "true" : "false");
	txt << QChar(30) << ((allowInsertingTaxInvoiceNumber) ? "true" : "false");
	txt << QChar(30) << ((newBCMustHaveTaxInvoiceNumber) ? "true" : "false");
	txt.flush();
	return buffer;
}

ModeDescription::ModeDescription()
	:mode(Modes::Search), sysfeed(-1), serializationOrder(), floatControl(false),
	previousDocument(0), attachNewDataToPrevious(false), clearBeforeAttachingNewData(false),
	allowInsertingTaxInvoiceNumber(false), newBCMustHaveTaxInvoiceNumber(false)
{
}

ModeDescription::ModeDescription(Modes md)
	: mode(md), sysfeed(-1), serializationOrder(), floatControl(false),
	previousDocument(0), attachNewDataToPrevious(false), clearBeforeAttachingNewData(false),
	allowInsertingTaxInvoiceNumber(false), newBCMustHaveTaxInvoiceNumber(false)
{
	switch (md)
	{
		case Modes::Invoices:
		{
			sysfeed = 1233;
		}
		break;
		case Modes::Simple:
		{
			sysfeed = 12095;
		}
		break;
	}
}

ModeDescription::ModeDescription(QString& serialized)
	: mode(Modes::Search), sysfeed(-1), serializationOrder(), floatControl(false),
	previousDocument(0), attachNewDataToPrevious(false), clearBeforeAttachingNewData(false),
	allowInsertingTaxInvoiceNumber(false), newBCMustHaveTaxInvoiceNumber(false)
{
	deserialize(serialized);
}

ModeDescription::ModeDescription(Modes m, int sf, QList<int>& sO, bool fc, long long int prevdoc, 
	bool andtp, bool cband, bool aITIN, bool NBMHTIN)
	: mode(m), sysfeed(sf), serializationOrder(sO), floatControl(fc), previousDocument(prevdoc),
	attachNewDataToPrevious(andtp), clearBeforeAttachingNewData(cband),
	allowInsertingTaxInvoiceNumber(aITIN), newBCMustHaveTaxInvoiceNumber(NBMHTIN)
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

const QList<int> & ModeDescription::getSerializationOrder() const
{
	return serializationOrder;
}

bool ModeDescription::requiresFloatControl() const
{
	return floatControl;
}

bool ModeDescription::requiresAttachingToPreviousDoc() const
{
	return attachNewDataToPrevious;
}

bool ModeDescription::mustClearBeforeAttaching() const
{
	return clearBeforeAttachingNewData;
}

long long int ModeDescription::getPreviousDocNumber() const
{
	return previousDocument;
}

bool ModeDescription::isInsertingTaxInvoiceNumAllowed() const
{
	return allowInsertingTaxInvoiceNumber;
}

bool ModeDescription::isForbiddenInsertingWithoutTaxInvoice() const
{
	return newBCMustHaveTaxInvoiceNumber;
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

void ModeDescription::setAttachingToPrevDoc(bool v)
{
	attachNewDataToPrevious = v;
}

void ModeDescription::setCleanBeforeAttaching(bool v)
{
	clearBeforeAttachingNewData = v;
}

void ModeDescription::setPreviousDocument(long long int nrdoc)
{
	previousDocument = nrdoc;
}

void ModeDescription::setInsertingTaxNumber(bool aITIN)
{
	allowInsertingTaxInvoiceNumber = aITIN;
}

void ModeDescription::setForbiddingInsertingWithoutTaxInvoice(bool nBCNHTIN)
{
	newBCMustHaveTaxInvoiceNumber = nBCNHTIN;
}
