#pragma once
#include <QtCore/QDateTime>
#include <QtCore/QVector>
#include <dataproviders/TableHandlers.h>
#include <QtSql/QSqlQuery>
#include <memory>
/*
		Reinterpret update:
			Now barcodes are polymorthic. Any descendant of abs_barcode can be written into db and can be used in
		const-interface operations in any multibranch widget.

		Core class of this file is abs_barcode. It allows barcodes to have similar interfaces - check it's desctiption
		BarcodeUniform is simple form of inventarisation data structure. It's main field is QUANTITY and which is stored in
		it's own table handler.
		BarcodeWithPrice is more complex, it holds 4 values of different prices and is more flexible.

		All util members are stored in it's own namespace. It contains barcodeMembers enum which allows to choose custom order of
		element drop. Also there is stored technical enum of barcodetypes, which defines inner type of barcode which is usefull
		for RTTI.

*/
namespace barcodeUtil
	// This namespace stores usefull but rarely used tools for handling abs_barcodes
{
	// this enum holds any barcode field which exists
	enum barcodeMembers {
		stringView, scanSessions, Quantity, AddDate, ExpDate,
		Comment, isUploaded, genPr, disPr, secPr, unnPr
	};

	extern const int TOTAL_CSV_LEN;
	extern const QStringList CSV_FIELD_NAMES;
	extern const QString CSV_STRING_TEMPLATE;
	extern const QString CSV_HEADER_STRING;
	extern const QString CSV_BARCODE_HEADER;
	extern const QString CSV_NONBARCODE_OP_TEMPLATE;
	// These vectors are holding default member orders
	extern QVector<barcodeMembers> defaultBarcodeUniform;
	extern QVector<barcodeMembers> defaultPricedBarcode;
	// This enum holds barcode types which are used to define which descendant of the abs_barcode is stored in pointer
	enum barcodetypes { abstractBc, uniformBc, pricedBc };
}
class abs_barcode
	// this class provides interface for any barcode and some common members
{
protected:
	virtual QString toSql(QVector<barcodeUtil::barcodeMembers> mems) = 0;
	virtual QString toSql() = 0;
	virtual const abs_table_handler* assocTable(int) = 0;
	barcodeUtil::barcodetypes mytype;

public:
	QString asString;			//	String representation of barcode. Is used to distinct one barcode from another.
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	int scanningSessions;				//	int value - quantity of scans
	QDateTime expDate;		//	datetime of expiration. Is needed only for sending - no modifications after user input
	QString comment;		//	comment to barcode

	explicit abs_barcode(QString barcode, barcodeUtil::barcodetypes type = barcodeUtil::abstractBc,
		QDateTime adddt = QDateTime::currentDateTime(), int scsess = 0, QDateTime expdt = QDateTime::currentDateTime(), QString comm = "");
	const abs_table_handler* getAssociatedTable(int subtype);
	QString asSqlInsertion(QVector<barcodeUtil::barcodeMembers> members);
	QString asSqlInsertion();
	virtual bool isValid() const;
	virtual QString formatedView(QString sep, QString dform) const = 0;
	virtual QString maximumInfoView(QString sep, QString dform) const = 0;
	virtual QString normalizedCsvView() const = 0;
	bool operator==(abs_barcode& bc);
	int myType() const;
};

typedef std::shared_ptr <abs_barcode> sharedabc; // This typedef provides smart pointer to abstract barcode. use it for polymorthic ops.

class BarcodeUniform : public abs_barcode
	// This class holds simple representation of inventarization barcode
{
protected:
	virtual QString toSql(QVector<barcodeUtil::barcodeMembers> mms = barcodeUtil::defaultBarcodeUniform) override;
	virtual QString toSql() override;
	const abs_table_handler* assocTable(int subtype) override;
public:
	int quantity;	//	quantity of items under this barcode
	//Default
	BarcodeUniform();
	// Use this constructor if you know this data already
	BarcodeUniform(QString& qs, int quant = 0, QDateTime add = QDateTime::currentDateTime());
	// Constuctor for literals
	BarcodeUniform(QString qs, int quant = 0);
	//	Query constructor is used for deserialization
	BarcodeUniform(QSqlQuery* q);

	virtual bool isValid() const override;
	virtual QString formatedView(QString sep, QString dform) const override;
	virtual QString maximumInfoView(QString sep, QString dform) const override;
	virtual QString normalizedCsvView() const override;
};
class BarcodeWithPrice : public abs_barcode {
protected:
	// Inherited via abs_barcode
	virtual QString toSql(QVector<barcodeUtil::barcodeMembers> mems) override;
	virtual QString toSql() override;
	virtual const abs_table_handler* assocTable(int) override;
public:
	int generalPrice;
	int discountPrice;
	int secondaryPrice;
	int unnecessaryPrice;

	BarcodeWithPrice();
	BarcodeWithPrice(QString qs, int gP = 0, int dP = 0);
	BarcodeWithPrice(QSqlQuery* q);

	virtual QString formatedView(QString sep, QString dform) const override;
	virtual QString maximumInfoView(QString sep, QString dform) const override;
	virtual bool isValid() const override;

	bool operator==(const BarcodeWithPrice& b2) const;
	BarcodeWithPrice& operator=(const BarcodeWithPrice& bc);
	virtual QString normalizedCsvView() const override;
};

// Comparator by string representation
bool operator==(const BarcodeUniform& b1, const BarcodeUniform& b2);
// Comparator by AddDate field
bool byAddDate(const sharedabc& b1, const sharedabc& b2);
// adder
int operator+(const BarcodeUniform& b1, const BarcodeUniform& b2);
// adder with pure quantity
int operator+(const BarcodeUniform& b1, int b2);
// hasher
uint qHash(const BarcodeUniform& b1);
// Counts summ of all barcode's quantity
int countAllBarcodesQuantity(QVector<BarcodeUniform>& v);
int countAllBarcodesQuantity(QVector<sharedabc>& v);
// Counts sessions summ for barcode with given link in vector
int countSessionsFound(QVector<BarcodeUniform>& v, BarcodeUniform& b);
// Counts summ of all barcodes similar to provided
int countAllFound(QVector<BarcodeUniform>& v, BarcodeUniform& b);
// Counts summ of unique barcodes in vector
int countUniqueBarcodesIn(QVector<BarcodeUniform>& v);
int countUniqueBarcodesIn(QVector<sharedabc>& v);