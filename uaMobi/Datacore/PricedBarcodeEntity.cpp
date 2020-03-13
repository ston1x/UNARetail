#include "PricedBarcodeEntity.h"
#include <QtSql/qsqlquery.h>
#include <QVariant>

static QString tableDefinition(QStringLiteral("( barcode TEXT,"
		"scanedtime datetime,"
		"generalPrice number,"
		"discountPrice number,"
		"secondaryPrice number,"
		"unnecessaryPrice number,"
		"uploaded number,"
		"expDateTime datetime,"
		"expComment TEXT,"
		"scanedSessions number"
	")"));
static QStringList tableFields
{
		QStringLiteral("barcode"),
		QStringLiteral("scanedtime"),
		QStringLiteral("generalPrice"),
		QStringLiteral("discountPrice"),
		QStringLiteral("secondaryPrice"),
		QStringLiteral("unnecessaryPrice"),
		QStringLiteral("uploaded"),
		QStringLiteral("expDateTime"),
		QStringLiteral("expComment"),
		QStringLiteral("scanedSessions")
};


static TemplatedTableHandler* barcodeTableHandler(
	new TemplatedTableHandler(
		"pricedbcs",
		tableDefinition,
		tableFields
	)
);

QString normalizePrice(int num)
{
	QString asnum = QString::number(num);
	if (asnum.isEmpty())
	{
		return asnum;
	}
	if (num > 100)
	{
		return asnum.chopped(2) + "," + asnum.right(2);
	}
	else
	{
		return "0," + asnum.rightJustified(2, '0');
	}
}

QString PricedBarcodeEntity::_toSql() const
{
	return "( '" + barcode + "' , '" + addDate.toString(datetimeDBEncoding) + 
		"' , " + QString::number(generalPrice) + " , "
		+ QString::number(discountPrice) + " , " +
		QString::number(secondaryPrice) + " , " +
		QString::number(unnecessaryPrice) + " , 1 , '" 
		+ expDate.toString(datetimeDBEncoding) + 
		"' , '" + comment + "' , " +
		QString::number(scanningSessions) + ")";
}

const TemplatedTableHandler* PricedBarcodeEntity::_assocTable() const
{
	return barcodeTableHandler;
}

QString PricedBarcodeEntity::_formatedView(QString sep, QString dform) const
{
	QString formated;
	formated = barcode + "\n" + sep + normalizePrice(generalPrice) + sep
		+ normalizePrice(discountPrice) + sep + normalizePrice(secondaryPrice)
		+ sep + normalizePrice(unnecessaryPrice) + sep;
	if (dform.isEmpty())
	{
		formated += expDate.time().toString();
	}
	else
	{
		formated += expDate.time().toString(dform);
	}
	formated += "\n" + comment.leftJustified(((comment.length() > 20) ? 20 : comment.length()), QChar(' '), true);
	return formated;
}

QString PricedBarcodeEntity::_maximumInfoView(QString sep, QString dform) const
{
	return formatedView(sep, dform);
}

QString PricedBarcodeEntity::_normalizedCsvView() const
{
	return barcodeUtil::CSV_BARCODE_STR_TEMPLATE.arg(barcode).arg(addDate.toString(datetimeDBEncoding)).arg(
		generalPrice).arg(expDate.toString(datetimeDBEncoding)).arg(
			comment).arg(discountPrice).arg(secondaryPrice).arg(
				unnecessaryPrice).arg("").arg("");
}

bool PricedBarcodeEntity::_isValid() const
{
	return generalPrice > 0 || discountPrice > 0;
}

bool PricedBarcodeEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	barcode = q->value(0).toString();
	addDate = QDateTime::fromString(q->value(1).toString(), datetimeDBEncoding);
	generalPrice = q->value(2).toInt();
	discountPrice = q->value(3).toInt();
	secondaryPrice = q->value(4).toInt();
	unnecessaryPrice = q->value(5).toInt();
	expDate = QDateTime::fromString(q->value(6).toString(), datetimeDBEncoding);
	comment = q->value(7).toString();
	scanningSessions = q->value(8).toInt();
	return true;
}

AbsEntity* PricedBarcodeEntity::_clone() const
{
	return new PricedBarcodeEntity(*this);
}

PricedBarcodeEntity::PricedBarcodeEntity(QString Barcode, QDateTime adddt, int scsess,
	QDateTime expdt, QString comm, int gP, int dP, int sP, int unP)
	:AbsEntity(int(barcodeUtil::barcodetypes::pricedBc)),  barcode(Barcode),
	addDate(adddt), scanningSessions(scsess), expDate(expdt), comment(comm), 
	generalPrice(gP), discountPrice(dP), secondaryPrice(sP), unnecessaryPrice(unP)
{
}

QString PricedBarcodeEntity::_getName() const
{
	return barcode;
}

int PricedBarcodeEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case -1:
	case barcodeUtil::generalPrice:
		return generalPrice;
	case barcodeUtil::discountPrice:
		return discountPrice;
	default:
		return 0;
	}
}

bool PricedBarcodeEntity::deepCompare(AbsEntity* other) const
{
	return AbsEntity::deepCompare(other) && other->getName() == barcode;
}

void PricedBarcodeEntity::_invalidate()
{
	generalPrice = 0;
	discountPrice = 0;
}

int PricedBarcodeEntity::_getHeight() const
{
	return 2 + comment.count("\n");
}
