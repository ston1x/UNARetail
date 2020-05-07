#include "PricedBarcodeEntity.h"
#include <QtSql/qsqlquery.h>
#include <QVariant>
#include "widgets/utils/ElementsStyles.h"
#include <cmath>
static QString tableDefinition(QStringLiteral("("
		"id number PRIMARY KEY NOT NULL,"
		"barcode TEXT,"
		"scanedtime datetime,"
		"generalPrice number,"
		"discountPrice number,"
		"secondaryPrice number,"
		"unnecessaryPrice number,"
		"uploaded number,"
		"expDateTime datetime,"
		"expComment TEXT"
	")"));
QStringList _initTPBF()
{
        QStringList t;
            t<<
        QStringLiteral("id")<<
        QStringLiteral("barcode")<<
        QStringLiteral("scanedtime")<<
        QStringLiteral("generalPrice")<<
        QStringLiteral("discountPrice")<<
        QStringLiteral("secondaryPrice")<<
        QStringLiteral("unnecessaryPrice")<<
        QStringLiteral("uploaded")<<
        QStringLiteral("expDateTime")<<
        QStringLiteral("expComment");
            return t;
}

static QStringList tableFields(_initTPBF());


static TemplatedTableHandler* barcodeTableHandler(
	new TemplatedTableHandler(
		"pricedbcs",
		tableDefinition,
		tableFields,
		0
	)
);

QString normalizePrice(double num)
{
	return QString::number(num, 'g',4);
}

QString PricedBarcodeEntity::_toSql() const
{
	return "( " + serializeId() + ",'" + barcode + "' , '" + addDate.toString(datetimeDBEncoding) + 
		"' , " + QString::number(generalPrice) + " , "
		+ QString::number(discountPrice) + " , " +
		QString::number(secondaryPrice) + " , " +
		QString::number(unnecessaryPrice) + " , 1 , '" 
		+ expDate.toString(datetimeDBEncoding) + 
		"' , '" + QString(comment).replace("'", "''").replace("\"", "\"\"") + "')";
}

const TemplatedTableHandler* PricedBarcodeEntity::_assocTable() const
{
	return barcodeTableHandler;
}

QString PricedBarcodeEntity::_formatedView(QString sep, QString dform) const
{
	return comment + sep + barcode + sep + normalizePrice(generalPrice) + sep
		+ normalizePrice(discountPrice) + sep + normalizePrice(secondaryPrice)
		+ sep + normalizePrice(unnecessaryPrice);
}

QString PricedBarcodeEntity::_maximumInfoView(QString sep, QString dform) const
{
	QString formated;
	formated = barcode + "\n" + sep + normalizePrice(generalPrice) + sep
		+ normalizePrice(discountPrice) + sep + normalizePrice(secondaryPrice)
		+ sep + normalizePrice(unnecessaryPrice) + sep;
	if (dform.isEmpty())
	{
		formated += expDate.toString(timeDBEncoding);
	}
	else
	{
		formated += expDate.toString(dform);
	}
	formated += "\n" + comment;
	return formated;
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
	return !barcode.isEmpty() &&(generalPrice > 0 || discountPrice > 0);
}

bool PricedBarcodeEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	GUID = q->value(0).toLongLong();
	barcode = q->value(1).toString();
	addDate = QDateTime::fromString(q->value(2).toString(), datetimeDBEncoding);
	generalPrice = q->value(3).toDouble();
	discountPrice = q->value(4).toDouble();
	secondaryPrice = q->value(5).toDouble();
	unnecessaryPrice = q->value(6).toDouble();
	expDate = QDateTime::fromString(q->value(8).toString(), datetimeDBEncoding);
	comment = q->value(9).toString();
	return true;
}

AbsEntity* PricedBarcodeEntity::_clone() const
{
	return new PricedBarcodeEntity(*this);
}

PricedBarcodeEntity::PricedBarcodeEntity(QString Barcode, QDateTime adddt, 
	QDateTime expdt, QString comm, double gP, double dP, double sP, double unP)
	:AbsEntity(int(barcodeUtil::barcodetypes::pricedBc)),  barcode(Barcode),
	addDate(adddt), expDate(expdt), comment(comm), 
	generalPrice(gP), discountPrice(dP), secondaryPrice(sP), unnecessaryPrice(unP)
{
}

QString PricedBarcodeEntity::_getName() const
{
	return barcode;
}

double PricedBarcodeEntity::_getEnumerable(int role) const
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
	return (std::ceil(double(barcode.count() + comment.count()
		+ datetimeDBEncoding.count()
		) / double(AppFonts->howMuchCharacterFitsIntoScreen())) + comment.count("\n") + 2.0);
}

const QStringList& PricedBarcodeEntity::_getFields() const
{
	return tableFields;
}

QString PricedBarcodeEntity::_fullComparationQuery() const
{
	return " barcode = '" + barcode + "' and generalPrice = " + QString::number(generalPrice)+
		" and discountPrice = " + QString::number(discountPrice);
}

void PricedBarcodeEntity::_setEnumerable(int role, double value)
{
	switch (role)
	{
	case 0:
		generalPrice = value;
		break;
	case 1:
		discountPrice = value;
		break;
	case 2:
		secondaryPrice = value;
		break;
	case 3:
		unnecessaryPrice = value;
		break;
	}

}
