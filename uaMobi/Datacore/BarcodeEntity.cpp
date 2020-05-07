#include "BarcodeEntity.h"
#include "dataproviders/TableHandlers.h"
#include <QtSql/qsqlquery.h>
#include <QVariant>
#include "widgets/utils/ElementsStyles.h"
#include <cmath>


static QString tableDefinition ( QStringLiteral( "("
		"id number PRIMARY KEY NOT NULL,"
		"barcode TEXT,"
		"scanedtime datetime,"
		"quantity number,"
		"uploaded number,"
		"expDateTime datetime,"
		"expComment TEXT"
	")"));
QStringList _initTBF()
{
    QStringList t;
    t<<
    QStringLiteral("id")<<
    QStringLiteral("barcode")<<
    QStringLiteral("scanedtime")<<
    QStringLiteral("quantity")<<
    QStringLiteral("uploaded")<<
    QStringLiteral("expDateTime")<<
    QStringLiteral("expComment");
    return t;
}

static QStringList tableFields(_initTBF());


static TemplatedTableHandler* barcodeTableHandler(
	new TemplatedTableHandler(
		"barcodes",
		tableDefinition,
		tableFields,
		0
	)
);



QString BarcodeEntity::_toSql() const
{
	return "(" + serializeId() + ",'" + barcode + "' , '" + addDate.toString(datetimeDBEncoding) 
		+ "', " + QString::number(quantity)  + " , " + QString::number(isUploaded) + " , '" 
		+ expDate.toString(datetimeDBEncoding)
		 + "' , '" + QString(comment).replace("'", "''").replace("\"", "\"\"") + "')";
}

const TemplatedTableHandler* BarcodeEntity::_assocTable() const
{
	return barcodeTableHandler;
}

QString BarcodeEntity::_formatedView(QString sep, QString dform) const
{
	QString formated;
	if (!comment.isEmpty())
	{
		formated = comment.leftJustified(15) + sep + QString::number(quantity);
	}
	else
	{
		formated = barcode.leftJustified(15) + sep + QString::number(quantity);
	}
	return formated;
}

QString BarcodeEntity::_maximumInfoView(QString sep, QString dform) const
{
	return barcode + "\n" + sep + QString::number(quantity) + sep +
		addDate.toString(datetimeDBEncoding) + "\n"
		+ comment;
}

QString BarcodeEntity::_normalizedCsvView() const
{
	return barcodeUtil::CSV_BARCODE_STR_TEMPLATE.arg(barcode).arg(addDate.toString()).arg(
		quantity).arg(expDate.toString()).arg(
			comment).arg("").arg("").arg(
				"").arg("").arg("");
}

bool BarcodeEntity::_isValid() const
{
	return quantity != 0 && (!barcode.isEmpty());
}

BarcodeEntity::BarcodeEntity(QString Barcode, QDateTime adddt, int isupl, 
	QDateTime expdt, QString comm, double Quantity)
	: AbsEntity(int(barcodeUtil::barcodetypes::uniformBc)),
	barcode(Barcode), addDate(adddt), isUploaded(isupl),
	expDate(expdt), comment(comm), quantity(Quantity)
{
}

BarcodeEntity::BarcodeEntity(QString bc, QString comm)
: AbsEntity(int(barcodeUtil::barcodetypes::uniformBc)),
barcode(bc), addDate(QDateTime::currentDateTime()), isUploaded(0),
expDate(QDateTime::currentDateTime()), comment(comm), quantity(0)
{

}

unsigned int BarcodeEntity::getEnumerableFieldIndex()
{
	return 3;
}

QString BarcodeEntity::_getName() const
{
	return barcode;
}

double BarcodeEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case -1:
	case int(barcodeUtil::barcodeenumerables::quantity):
		return double(quantity);
	default:
		return 0;
	}
}

void BarcodeEntity::_invalidate()
{
	quantity = 0;
}

int BarcodeEntity::_getHeight() const
{
	return (std::ceil(double(barcode.count() + comment.count() 
		+ datetimeDBEncoding.count() 
		)  / double(AppFonts->howMuchCharacterFitsIntoScreen())) + comment.count("\n") + 2);
}

const QStringList& BarcodeEntity::_getFields() const
{
	return tableFields;
}

QString BarcodeEntity::_fullComparationQuery() const
{
	return "barcode = '" + barcode + "' and quantity = " + QString::number(quantity);
}

void BarcodeEntity::_setEnumerable(int role, double value)
{
	if (role == 0)
	{
		quantity = value;
	}
}

bool BarcodeEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	GUID = q->value(0).toLongLong();
	barcode = q->value(1).toString();
	addDate =
		QDateTime::fromString(q->value(2).toString(), datetimeDBEncoding);
	quantity = q->value(3).toDouble();
	isUploaded = q->value(4).toInt();
	expDate =
		QDateTime::fromString(q->value(5).toString(), datetimeDBEncoding);
	comment = q->value(6).toString();
	return true;
}

AbsEntity* BarcodeEntity::_clone() const
{
	return new BarcodeEntity(*this);
}

bool BarcodeEntity::deepCompare(AbsEntity* other) const
{
	return AbsEntity::deepCompare(other) && (other->getName() == barcode);
}
