#include "BarcodeUniform.h"
#include <QtCore/QSet>
#include <QtCore/QVariant>
#include <QtSql/QSqlRecord>
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include <mutex>

static const QString dbDateFormat{ "dd.MM.yyyy hh.mm.ss" };
namespace barcodeUtil {
	QVector<barcodeMembers> defaultBarcodeUniform{ stringView, AddDate, Quantity, isUploaded, ExpDate, Comment, scanSessions };
	QVector<barcodeMembers> defaultPricedBarcode{ stringView, AddDate, genPr, disPr, secPr, unnPr, isUploaded, ExpDate, Comment, scanSessions };
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

	const int TOTAL_CSV_LEN = 12;
    const QStringList CSV_FIELD_NAMES{ "OPERATION", "MODE", "BARCODE", "ADDDATE", "QUANTITY|PRICE", "EXPDATE", "COMMENT", "BARCODESP1","BARCODESP2","BARCODESP3","RESERVED1", "RESERVED2" };
    const QString CSV_STRING_TEMPLATE = "%0;%1;\"%2\";%3;%4;%5;\"%6\";%7;%8;%9;%10;%11\r\n" ;
    const QString CSV_BARCODE_STR_TEMPLATE = "\"%0\";\"%1\";\"%2\";\"%3\";\"%4\";%5;%6;%7;%8;%9";
    const QString CSV_HEADER_STRING = "OPERATION;MODE;BARCODE;ADDDATE;QUANTITY|PRICE;EXPDATE;COMMENT;BARCODESP1;BARCODESP2;BARCODESP3;RESERVED1;RESERVED2\r\n";
    const QString CSV_BARCODE_HEADER = "%0;%1;%2\r\n";
    const QString CSV_NONBARCODE_OP_TEMPLATE = "%0;%1;;;;;;;;;%2;%3\r\n";
}
using namespace barcodeUtil;
bool operator==(const BarcodeUniform& b1, const BarcodeUniform& b2)
// This function provides comparation for two uniform barcodes
{
	return b1.asString == b2.asString;		//	For better performance uses asint in comparation
}

bool byAddDate(const sharedabc& b1, const sharedabc& b2)
// This function is usefull for sorting by add date
{
	return b1->addDate > b2->addDate;
}

int operator+(const BarcodeUniform& b1, const BarcodeUniform& b2)
// This is default adder for uniform barcodes - it summs quantity
{
	return b1.quantity + b2.quantity;
}

int operator+(const BarcodeUniform& b1, int b2)
// This is method-adder - it summs quantity
{
	return b1.quantity + b2;
}

uint qHash(const BarcodeUniform& b1)
// This is hasher of uniform barcode
{
	return qHash(b1.asString);
}

int countAllBarcodesQuantity(QVector<BarcodeUniform>& v)
// Sums quantity of all uniform barcodes in the vector
{
	int sum = 0;
	auto start = v.begin();
	while (start != v.end())
	{
		sum += start->quantity;
		++start;
	}

	return sum;
}

int countAllBarcodesQuantity(QVector<sharedabc>& v)
// Sums quantity as polymorthic operation.
{
	if (v.count() > 0)
	{
		switch (v.first()->myType())
		{
		case uniformBc:
		{int sum = 0;
		std::shared_ptr<BarcodeUniform> t;
		auto start = v.begin();
		while (start != v.end())
		{
			t = std::dynamic_pointer_cast<BarcodeUniform>(*start);
			sum += t->quantity;
			++start;
		}
		return sum;
		}
		case pricedBc:
		{
			int sum = 0;
			std::shared_ptr<BarcodeWithPrice> t;
			auto start = v.begin();
			while (start != v.end())
			{
				t = std::dynamic_pointer_cast<BarcodeWithPrice>(*start);
				sum += t->generalPrice;
				++start;
			}
			return sum;
		}
		}
	}
	return 0;
}

int countSessionsFound(QVector<BarcodeUniform>& v, BarcodeUniform& b)
// sums sessions of scanning for particular barcode. Expencive
{
	int sum = 0;
	auto start = v.begin();
	while (start != v.end())
	{
		if (start->asString == b.asString) {
			sum += start++->scanningSessions;
		}
		else
			++start;
	}
	return sum;
}

int countAllFound(QVector<BarcodeUniform>& v, BarcodeUniform& b)
// counts all found barcodes. O(n)
{
	int sum = 0;
	auto start = v.begin();
	while (start != v.end())
	{
		if (start->asString == b.asString) {
			sum += start++->quantity;
		}
		else
			++start;
	}
	return sum;
}

int countUniqueBarcodesIn(QVector<BarcodeUniform>& v)
// counts all unique barcodes. Expencive: uses set
{
	return QSet<BarcodeUniform>::fromList(v.toList()).count();
}
int countUniqueBarcodesIn(QVector<sharedabc>& v)
// counts all unique barcodes polymorthic
{
	QSet<QString> unics;
	auto start = v.begin();
	while (start != v.end())
	{
		unics.insert((*start)->asString);
		++start;
	}
	return unics.count();
}
QString BarcodeUniform::toSql(QVector<barcodeUtil::barcodeMembers> mms)
// converts barcode to string containing it's fields. Usually is used in sql queries. This version is slower
{
	using namespace barcodeUtil;
	QString result("( ");
	if (mms.isEmpty())
	{
		mms = defaultBarcodeUniform;
	}

	int end = mms.count();
	if (end <= 0)
		return result + ")";
	for (int i = 0; i < end; ++i)
	{
		switch (mms.at(i))
		{
		case stringView:
			result += " '" + asString + "' ,";
			break;
		case scanSessions:
			result += " " + QString::number(scanningSessions) + " ,";
			break;
		case Quantity:
			result += " " + QString::number(quantity) + " ,";
			break;
		case AddDate:
			result += " '" + addDate.toString(dbDateFormat) + "' ,";
			break;
		case ExpDate:
			result += " '" + expDate.toString(dbDateFormat) + "' ,";
			break;
		case Comment:
			result += " '" + comment + "' ,";
			break;
		case isUploaded:
			result += " 1,";
			break;
		default:
			break;
		}
	}
	result.chop(1);
	result += ")";

	return result;
}

QString BarcodeUniform::toSql()
// converts barcode into string containing all it's fields. usualy is used in sql queries. Fast.
{
	return "( '" + asString + "' , '" + addDate.toString(dbDateFormat) + "' , "
		+ QString::number(quantity) + " , 1 , '" + expDate.toString(dbDateFormat) + "' , '" + comment.replace("'","''") + "' , " +
		QString::number(scanningSessions) + " )";
}

const abs_table_handler* BarcodeUniform::assocTable(int subtype)
// Returns table which is associated with this barcode's type and with given table's definition
{
#ifdef DEBUG
	detrace_METHEXPL("in uniform assoc");
#endif
	switch (subtype)
	{
	case Scanned:
		return tables[Scanned];
	case Uploaded:
		return tables[Uploaded];
	case TempScan:
		return tables[TempScan];
	default:
		return nullptr;
	}
}

BarcodeUniform::BarcodeUniform()
	: abs_barcode(QString(), uniformBc), quantity(0)
{
}

BarcodeUniform::BarcodeUniform(QString& qs, int quant, QDateTime add)
	: abs_barcode(qs, uniformBc, add), quantity(quant)
{
}

BarcodeUniform::BarcodeUniform(QString qs, int quant)
	: abs_barcode(qs, uniformBc), quantity(quant)
{
}

BarcodeUniform::BarcodeUniform(QSqlQuery* q)
	: abs_barcode("", uniformBc)
{
	if (q->record().count() != 6 || !q->isActive())
		return;
	asString = q->value(0).toString();
	addDate =
		QDateTime::fromString(q->value(1).toString(), dbDateFormat);
	quantity = q->value(2).toInt();
	expDate =
		QDateTime::fromString(q->value(3).toString(), dbDateFormat);
	comment = q->value(4).toString();
	scanningSessions = q->value(5).toInt();
}

bool BarcodeUniform::isValid() const
// Checks if there is some sense in storing this barcode
{
	return quantity != 0;
}

QString BarcodeUniform::formatedView(QString sep, QString dform) const
// returns shortened formatted view of this barcode using separator and data format
{
	QString formated;
	if (dform.isEmpty())
	{
		formated = asString + sep + QString::number(quantity) + sep
			+ expDate.time().toString();
		if (!comment.isEmpty())
		{
			formated += "\n" + comment.chopped((comment.length() > 20) ? 20 : comment.length());
		}
	}
	else
	{
		formated = asString + sep + QString::number(quantity) + sep
			+ expDate.time().toString(dform);
		if (!comment.isEmpty())
		{
			formated += "\n" + comment.leftJustified(((comment.length() > 20) ? 20 : comment.length()), QChar(' '), true);
		}
	}
	return formated;
}

QString BarcodeUniform::maximumInfoView(QString sep, QString dform) const
// returns all data this particular barcode is willing to show
{
	return asString + "\n" + sep + QString::number(quantity) + sep + QString::number(scanningSessions) + sep +
		addDate.toString(dbDateFormat) + "\n"
		+ comment;
}

QString BarcodeUniform::normalizedCsvView() const
{
	return CSV_BARCODE_STR_TEMPLATE.arg(asString).arg(addDate.toString()).arg(
		quantity).arg(expDate.toString()).arg(
			comment).arg("").arg("").arg(
				"").arg("").arg("");
}

abs_barcode::abs_barcode(QString barcode, barcodeUtil::barcodetypes type, QDateTime adddt, int scsess, QDateTime expd, QString cmm)
	: asString(barcode), mytype(type), addDate(adddt), scanningSessions(scsess), expDate(expd), comment(cmm)
{
}

const abs_table_handler* abs_barcode::getAssociatedTable(int subtype)
// returns associated table of the barcode stored in this pointer
{
	return assocTable(subtype);
}

QString abs_barcode::asSqlInsertion(QVector<barcodeUtil::barcodeMembers> members)
// returns string which stores all fields. Usually is used in sql queries. This version allows custom positioning
{
	return toSql(members);
}

QString abs_barcode::asSqlInsertion()
// returns string which stores all fields. Usually is used in sql queries. This version is faster
{
	return toSql();
}

bool abs_barcode::isValid() const
// checks if there is sense of storing this barcode
{
	return false;
}

bool abs_barcode::operator==(abs_barcode& bc)
// Comparator of two barcodes. Polymorthic.
{
	return asString == bc.asString;
}

int abs_barcode::myType() const
// returns type of barcode stored in this pointer
{
	return mytype;
}

QString BarcodeWithPrice::toSql(QVector<barcodeUtil::barcodeMembers> mms)
// returns string which stores this barcode's field
{
	using namespace barcodeUtil;
	QString result("( ");
	if (mms.isEmpty())
	{
		mms = defaultPricedBarcode;
	}

	int end = mms.count();
	if (end <= 0)
		return result + ")";
	for (int i = 0; i < end; ++i)
	{
		switch (mms.at(i))
		{
		case stringView:
			result += " '" + asString + "' ,";
			break;
		case scanSessions:
			result += " " + QString::number(scanningSessions) + " ,";
			break;
		case AddDate:
			result += " '" + addDate.toString(dbDateFormat) + "' ,";
			break;
		case ExpDate:
			result += " '" + expDate.toString(dbDateFormat) + "' ,";
			break;
		case Comment:
			result += " '" + comment + "' ,";
			break;
		case isUploaded:
			result += " 1,";
			break;
		case genPr:
			result += " " + QString::number(generalPrice) + " ,";
			break;
		case disPr:
			result += " " + QString::number(discountPrice) + " ,";
			break;
		case secPr:
			result += " " + QString::number(secondaryPrice) + " ,";
			break;
		case unnPr:
			result += " " + QString::number(unnecessaryPrice) + " ,";
			break;
		default:
			break;
		}
	}
	result.chop(1);
	result += ")";

	return result;
}

QString BarcodeWithPrice::toSql()
// Is shortened version of toSql(mems)
{
	return "( '" + asString + "' , '" + addDate.toString(dbDateFormat) + "' , " + QString::number(generalPrice) + " , "
		+ QString::number(discountPrice) + " , " + QString::number(secondaryPrice) + " , " +
		QString::number(unnecessaryPrice) + " , 1 , '" + expDate.toString(dbDateFormat) + "' , '" + comment.replace("'", "''") + "' , " +
		QString::number(scanningSessions) + " )";
}

const abs_table_handler* BarcodeWithPrice::assocTable(int subtype)
// returns associated table which shifts subtype using offset enum
{
#ifdef DEBUG
	//detrace_METHEXPL("in price associated table: indexes " << Scanned + Priced << " " << Uploaded + Priced << " " );
	 //   detrace_METHEXPL(" " << TempScan + Priced << " while tables they are pointing at are: ");
	   // detrace_METHEXPL(tables[Scanned + Priced]->tableDesc() << tables[Uploaded + Priced]->tableDesc() << tables[TempScan + Priced]->tableDesc());
#endif
	switch (subtype)
	{
	case Scanned:
		return tables[Scanned + Priced];
	case Uploaded:
		return tables[Uploaded + Priced];
	case TempScan:
		return tables[TempScan + Priced];
	default:
		return nullptr;
	}
}

BarcodeWithPrice::BarcodeWithPrice()
	: abs_barcode("", pricedBc), generalPrice(0), discountPrice(0),
	secondaryPrice(0), unnecessaryPrice(0)
{
}

BarcodeWithPrice::BarcodeWithPrice(QString qs, int gP, int dP)
	: abs_barcode(qs, pricedBc), generalPrice(gP), discountPrice(dP),
	secondaryPrice(0), unnecessaryPrice(0)
{
}
BarcodeWithPrice::BarcodeWithPrice(QSqlQuery* q)
	: abs_barcode("", pricedBc)
{
#ifdef DEBUG
	// detrace_METHEXPL("in bwp cons:" << q->record().count() << " records and " <<  q->isActive() << " query");
#endif
	if (q->record().count() != 9 || !q->isActive())
		return;

	asString = q->value(0).toString();
	addDate = QDateTime::fromString(q->value(1).toString(), dbDateFormat);
	generalPrice = q->value(2).toInt();
	discountPrice = q->value(3).toInt();
	secondaryPrice = q->value(4).toInt();
	unnecessaryPrice = q->value(5).toInt();
	expDate = QDateTime::fromString(q->value(6).toString(), dbDateFormat);
	comment = q->value(7).toString();
	scanningSessions = q->value(8).toInt();
}
QString BarcodeWithPrice::formatedView(QString sep, QString dform) const
{
	QString formated;
	formated = asString + "\n" + sep + normalizePrice(generalPrice) + sep
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

QString BarcodeWithPrice::maximumInfoView(QString sep, QString dform) const
{
	return formatedView(sep, dform);
}

bool BarcodeWithPrice::isValid() const
{
	return generalPrice > 0 || discountPrice > 0;
}

bool BarcodeWithPrice::operator==(const BarcodeWithPrice& b2) const
{
	return asString == b2.asString;
}

BarcodeWithPrice& BarcodeWithPrice::operator=(const BarcodeWithPrice& bc)
{
	asString = bc.asString;
	addDate = bc.addDate;
	expDate = bc.expDate;
	generalPrice = bc.generalPrice;
	discountPrice = bc.discountPrice;
	secondaryPrice = bc.secondaryPrice;
	unnecessaryPrice = bc.unnecessaryPrice;
	comment = bc.comment;
	scanningSessions = bc.scanningSessions;
	return *this;
}

QString BarcodeWithPrice::normalizedCsvView() const
{
	return CSV_BARCODE_STR_TEMPLATE.arg(asString).arg(addDate.toString()).arg(
		generalPrice).arg(expDate.toString()).arg(
		comment).arg(discountPrice).arg(secondaryPrice).arg(
		unnecessaryPrice).arg("").arg("");
}
