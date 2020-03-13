#include "BarcodeEntity.h"
#include "dataproviders/TableHandlers.h"
#include <QtSql/qsqlquery.h>
#include <QVariant>

static QString tableDefinition ( QStringLiteral( "( barcode TEXT,"
		"scanedtime datetime,"
		"quantity number,"
		"uploaded number,"
		"expDateTime datetime,"
		"expComment TEXT"
	")"));

static QStringList tableFields
{
		QStringLiteral("barcode"),
		QStringLiteral("scanedtime"),
		QStringLiteral("quantity"),
		QStringLiteral("uploaded"),
		QStringLiteral("expDateTime"),
		QStringLiteral("expComment")
};


static TemplatedTableHandler* barcodeTableHandler(
	new TemplatedTableHandler(
		"barcodes",
		tableDefinition,
		tableFields
	)
);



QString BarcodeEntity::_toSql() const
{
	return "('" + barcode + "' , '" + addDate.toString(datetimeDBEncoding) 
		+ "', " + QString::number(quantity)  + " , " + QString::number(isUploaded) + " , '" 
		+ expDate.toString(datetimeDBEncoding)
		 + "' , '" + comment + "')";
}

const TemplatedTableHandler* BarcodeEntity::_assocTable() const
{
	return barcodeTableHandler;
}

QString BarcodeEntity::_formatedView(QString sep, QString dform) const
{
	QString formated;
	if (dform.isEmpty())
	{
		formated = barcode + sep + QString::number(quantity) + sep
			+ expDate.time().toString();
		if (!comment.isEmpty())
		{
			formated += "\n" + comment.chopped((comment.length() > 20) ? 20 : comment.length());
		}
	}
	else
	{
		formated = barcode + sep + QString::number(quantity) + sep
			+ expDate.time().toString(dform);
		if (!comment.isEmpty())
		{
			formated += "\n" + comment.leftJustified(((comment.length() > 20) ? 20 : comment.length()), QChar(' '), true);
		}
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
	return quantity != 0;
}

BarcodeEntity::BarcodeEntity(QString Barcode, QDateTime adddt, int isupl, 
	QDateTime expdt, QString comm, double Quantity)
	: AbsEntity(int(barcodeUtil::barcodetypes::uniformBc)),
	barcode(Barcode), addDate(adddt), isUploaded(isupl),
	expDate(expdt), comment(comm), quantity(Quantity)
{
}

unsigned int BarcodeEntity::getEnumerableFieldIndex()
{
	return 2;
}

QString BarcodeEntity::_getName() const
{
	return barcode;
}

int BarcodeEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case -1:
	case int(barcodeUtil::barcodeenumerables::quantity):
		return int(quantity);
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
	return 2 + comment.count("\n");
}

bool BarcodeEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	barcode = q->value(0).toString();
	addDate =
		QDateTime::fromString(q->value(1).toString(), datetimeDBEncoding);
	quantity = q->value(2).toDouble();
	isUploaded = q->value(3).toInt();
	expDate =
		QDateTime::fromString(q->value(4).toString(), datetimeDBEncoding);
	comment = q->value(5).toString();
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
