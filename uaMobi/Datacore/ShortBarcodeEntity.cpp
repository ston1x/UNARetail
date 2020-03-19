#include "ShortBarcodeEntity.h"
#include <QtSql/QSqlQuery>
#include <QVariant>

static QString tableDefinition(QStringLiteral("( barcode TEXT,"
	"code number,"
	"info TEXT,"
	"count TEXT"
	")"));

static QStringList tableFields
{
		QStringLiteral("barcode"),
		QStringLiteral("code"),
		QStringLiteral("info"),
		QStringLiteral("count")
};


static TemplatedTableHandler* barcodeTableHandler(
	new TemplatedTableHandler(
		"shortbcs",
		tableDefinition,
		tableFields
	)
);


ShortBarcodeEntity::ShortBarcodeEntity(QString Barcode, int cod, QString Info, QString Count)
	: AbsEntity(int(barcodeUtil::barcodetypes::shortBc)), barcode(Barcode), code(cod),
	info(Info), count(Count)
{
}

const TemplatedTableHandler* ShortBarcodeEntity::getTableHandler()
{
	return barcodeTableHandler;
}

const QString ShortBarcodeEntity::makeInsertionFromString(QString vals)
{
	return barcodeTableHandler->insert("("+vals+")");
}

QString ShortBarcodeEntity::_getName() const
{
	return barcode;
}

int ShortBarcodeEntity::_getEnumerable(int role) const
{
	switch (role)
	{
	case -1:
		return code;
	default:
		return 0;
	}
}

bool ShortBarcodeEntity::deepCompare(AbsEntity* other) const
{
	return AbsEntity::deepCompare(other) && other->getName() == barcode;
}

void ShortBarcodeEntity::_invalidate()
{
	barcode.clear();
}

const QStringList& ShortBarcodeEntity::_getFields() const
{
	return tableFields;
}

QString ShortBarcodeEntity::_toSql() const
{
	return "( '" + barcode + "'," + QString::number(code) + ",'" + info + "','"
		+ count + "')";
}

const TemplatedTableHandler* ShortBarcodeEntity::_assocTable() const
{
	return barcodeTableHandler;
}

QString ShortBarcodeEntity::_formatedView(QString sep, QString dform) const
{
	return barcode + sep + info;
}

QString ShortBarcodeEntity::_maximumInfoView(QString sep, QString dform) const
{
	return barcode + sep + info + sep + QString::number(code) + sep + count;
}

QString ShortBarcodeEntity::_normalizedCsvView() const
{
	return barcodeUtil::CSV_BARCODE_STR_TEMPLATE.arg(barcode).arg("").arg(
		count).arg(QString()).arg(
			info).arg("").arg("").arg(
				"").arg("").arg(code);
}

bool ShortBarcodeEntity::_isValid() const
{
	return !barcode.isEmpty();
}

bool ShortBarcodeEntity::_fromSql(QSqlQuery* q)
{
	if (!q->next())
		return false;
	barcode = q->value(0).toString();
	code = q->value(1).toInt();
	info = q->value(2).toString();
	count = q->value(3).toString();
	return true;
}

AbsEntity* ShortBarcodeEntity::_clone() const
{
	return new ShortBarcodeEntity(*this);
}
