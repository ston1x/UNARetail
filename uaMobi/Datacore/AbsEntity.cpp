#include "AbsEntity.h"

bool AbsEntity::deepCompare(AbsEntity* bc) const
{
	return mytype == bc->myType();
}

int AbsEntity::_getHeight() const
{
	return _getName().count("\n");
}

AbsEntity::AbsEntity(int type)
	: mytype(type)
{
}

const TemplatedTableHandler* AbsEntity::getAssociatedTable() const
{
	return _assocTable();
}


QString AbsEntity::asSqlInsertion() const
{
	return _toSql();
}

QString AbsEntity::maximizedView(QString sep, QString dform) const
{
	return _maximumInfoView(sep, dform);
}

QString AbsEntity::formatedView(QString sep, QString dform) const
{
	return _formatedView(sep,dform);
}

QString AbsEntity::normalizedCsvView() const
{
	return _normalizedCsvView();
}

bool AbsEntity::isValid() const
{
	return _isValid();
}


bool AbsEntity::operator==(AbsEntity* bc) const
{
	return deepCompare(bc);
}

bool AbsEntity::operator==(std::shared_ptr<AbsEntity> bc) const
{
	return deepCompare(&(*bc));
}

int AbsEntity::myType() const
{
	return mytype;
}

bool AbsEntity::fromSql(QSqlQuery* q)
{
	return _fromSql(q);
}

AbsEntity* AbsEntity::clone() const
{
	return _clone();
}

QString AbsEntity::getName() const
{
	return _getName();
}

int AbsEntity::getEnumerable(int role) const
{
	return _getEnumerable(role);
}

void AbsEntity::invalidate()
{
	_invalidate();
}

int AbsEntity::getHeight() const
{
	return _getHeight();
}


static const QString datetimeDBEncoding(QStringLiteral("dd.MM.yyyy hh.mm.ss"));
static const QString dateDBEncoding(QStringLiteral("dd.MM.yyyy"));
namespace barcodeUtil {
	static const int TOTAL_CSV_LEN = 12;
	static const QStringList CSV_FIELD_NAMES
	{
		QStringLiteral("OPERATION"),
		QStringLiteral("MODE"),
		QStringLiteral("BARCODE"),
		QStringLiteral("ADDDATE"),
		QStringLiteral("QUANTITY|PRICE"),
		QStringLiteral("EXPDATE"),
		QStringLiteral("COMMENT"),
		QStringLiteral("BARCODESP1"),
		QStringLiteral("BARCODESP2"),
		QStringLiteral("BARCODESP3"),
		QStringLiteral("RESERVED1"),
		QStringLiteral("RESERVED2")
	};
	static const QString CSV_STRING_TEMPLATE = 
		QStringLiteral("%0;%1;\"%2\";%3;%4;%5;\"%6\";%7;%8;%9;%10;%11\r\n");
	static const QString CSV_BARCODE_STR_TEMPLATE = 
		QStringLiteral("\"%0\";\"%1\";\"%2\";\"%3\";\"%4\";%5;%6;%7;%8;%9");
	static const QString CSV_HEADER_STRING = 
		QStringLiteral("OPERATION;MODE;BARCODE;ADDDATE;QUANTITY|PRICE;"
		"EXPDATE;COMMENT;BARCODESP1;BARCODESP2;BARCODESP3;RESERVED1;RESERVED2\r\n");
	static const QString CSV_BARCODE_HEADER = QStringLiteral("%0;%1;%2\r\n");
	static const QString CSV_NONBARCODE_OP_TEMPLATE = QStringLiteral("%0;%1;;;;;;;;;%2;%3\r\n");
}