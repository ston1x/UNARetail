#include "ShortBarcodeEntity.h"
#include <QtSql/QSqlQuery>
#include <QVariant>

static QString tableDefinition(QStringLiteral("("
	"id number PRIMARY KEY NOT NULL,"
	"barcode TEXT,"
	"code number,"
	"info TEXT,"
	"count TEXT"
	")"));

QStringList _initTSBF()
{
QStringList t;
t<<
    QStringLiteral("id")<<
        QStringLiteral("barcode")<<
        QStringLiteral("code")<<
        QStringLiteral("info")<<
        QStringLiteral("count");
return t;
}
static QStringList tableFields(_initTSBF());


static TemplatedTableHandler* barcodeTableHandler(
	new TemplatedTableHandler(
		"shortbcs",
		tableDefinition,
		tableFields,
		0
	)
);


ShortBarcodeEntity::ShortBarcodeEntity(QString Barcode, int cod, QString Info, QString Count)
	: AbsEntity(int(barcodeUtil::barcodetypes::shortBc)), barcode(Barcode), code(cod),
	info(Info), count(Count)
{
}

void ShortBarcodeEntity::clear()
{
	barcode.clear();
	code = 0;
	info.clear();
	count.clear();
	GUID = makeGUID();
}

const TemplatedTableHandler* ShortBarcodeEntity::getTableHandler()
{
	return barcodeTableHandler;
}

const QString ShortBarcodeEntity::makeInsertionFromString(QString vals)
{
	return barcodeTableHandler->insert("(" + QString::number(AbsEntity::makeGUID()) 
		+ ","+vals+")");
}

const QStringList & ShortBarcodeEntity::getFieldNames()
{
	return tableFields;
}

QSharedPointer<ShortBarcodeEntity> ShortBarcodeEntity::extractFromLine(QString line)
{
	QSharedPointer<ShortBarcodeEntity> toReturn(new ShortBarcodeEntity());
	QStringList split2lvl = line.split(",");
	switch (split2lvl.count())
	{
	case 4:
		toReturn->count = split2lvl.at(3);
		Q_FALLTHROUGH();
	case 3:
		toReturn->info = split2lvl.at(2);
		Q_FALLTHROUGH();
	case 2:
		toReturn->code = split2lvl.at(1).toInt();
		Q_FALLTHROUGH();
	case 1:
		toReturn->barcode = split2lvl.at(0);
		Q_FALLTHROUGH();
	default:
		break;
	}
	if (toReturn->barcode.isEmpty())
	{
		toReturn->barcode.fill('1', 1);
	}
	return toReturn;
}

QString ShortBarcodeEntity::_getName() const
{
	return barcode;
}

double ShortBarcodeEntity::_getEnumerable(int role) const
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

QString ShortBarcodeEntity::_fullComparationQuery() const
{
	return "barcode = '" + barcode + "'";
}

QString ShortBarcodeEntity::_toSql() const
{
	return "( " + serializeId() + ",'" + barcode + "'," + QString::number(code) + ",'" + info + "','"
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
	return info + sep  + "\n" + barcode + sep + QString::number(code) + sep + count;
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
	GUID = q->value(0).toLongLong();
	barcode = q->value(1).toString();
	code = q->value(2).toInt();
	info = q->value(3).toString();
	count = q->value(4).toString();
	return true;
}

AbsEntity* ShortBarcodeEntity::_clone() const
{
	return new ShortBarcodeEntity(*this);
}
