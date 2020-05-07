#include "AbsEntity.h"
#include <QDateTime>
bool AbsEntity::deepCompare(AbsEntity* bc) const
{
	return mytype == bc->myType() && getId() == bc->getId();
}

void AbsEntity::_setEnumerable(int role, double value)
{
}

int AbsEntity::_getHeight() const
{
	return _getName().count("\n") + 1;
}

AbsEntity::AbsEntity(int type)
	: mytype(type), GUID(makeGUID())
{
}

AbsEntity::AbsEntity(int type, long long int guid)
	: mytype(type), GUID(guid)
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

QString AbsEntity::fullComparationQuery() const
{
	return _fullComparationQuery();
}

bool AbsEntity::isValid() const
{
	return _isValid();
}


bool AbsEntity::operator==(AbsEntity* bc) const
{
	return deepCompare(bc);
}

bool AbsEntity::operator==(QSharedPointer<AbsEntity> bc) const
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

double AbsEntity::getEnumerable(int role) const
{
	return _getEnumerable(role);
}

void AbsEntity::setEnumerable(int role, double value)
{
	_setEnumerable(role, value);
}

void AbsEntity::invalidate()
{
	_invalidate();
}

int AbsEntity::getHeight() const
{
	return _getHeight();
}

const QStringList& AbsEntity::getFields() const
{
	return _getFields();
}

long long int AbsEntity::makeGUID()
{
	long long int guid = QDateTime::currentMSecsSinceEpoch();
	guid <<= 10;
	unsigned char rmod = rand();
	guid += rmod;
	return guid;
}

long long int AbsEntity::getId() const
{
	return GUID;
}

QString AbsEntity::serializeId() const
{
	return QString::number(GUID);
}


const QString datetimeDBEncoding = "dd.MM.yyyy hh.mm.ss";
const QString dateDBEncoding(QStringLiteral("dd.MM.yyyy"));
const QString timeDBEncoding(QStringLiteral("hh.mm.ss"));
namespace barcodeUtil {
    const int TOTAL_CSV_LEN = 12;

    QStringList _initCFN()
    {
        QStringList t;
        t << QStringLiteral("OPERATION")<<
        QStringLiteral("MODE")<<
        QStringLiteral("BARCODE")<<
        QStringLiteral("ADDDATE")<<
        QStringLiteral("QUANTITY|PRICE")<<
        QStringLiteral("EXPDATE")<<
        QStringLiteral("COMMENT")<<
        QStringLiteral("BARCODESP1")<<
        QStringLiteral("BARCODESP2")<<
        QStringLiteral("BARCODESP3")<<
        QStringLiteral("RESERVED1")<<
        QStringLiteral("RESERVED2");
        return t;
    }

    const QStringList CSV_FIELD_NAMES(_initCFN());
    const QString CSV_STRING_TEMPLATE =
		QStringLiteral("%0;%1;\"%2\";%3;%4;%5;\"%6\";%7;%8;%9;%10;%11\r\n");
    const QString CSV_BARCODE_STR_TEMPLATE =
		QStringLiteral("\"%0\";\"%1\";\"%2\";\"%3\";\"%4\";%5;%6;%7;%8;%9");
    const QString CSV_HEADER_STRING =
		QStringLiteral("OPERATION;MODE;BARCODE;ADDDATE;QUANTITY|PRICE;"
		"EXPDATE;COMMENT;BARCODESP1;BARCODESP2;BARCODESP3;RESERVED1;RESERVED2\r\n");
    const QString CSV_BARCODE_HEADER = QStringLiteral("%0;%1;%2\r\n");
    const QString CSV_NONBARCODE_OP_TEMPLATE = QStringLiteral("%0;%1;;;;;;;;;%2;%3\r\n");
}
