#pragma once
#include <QString>
#include "dataproviders/TableHandlers.h"
#include <QVector>
#include <qmetatype.h>
#include <qsharedpointer.h>

#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromUtf8("" A "" , sizeof(A)-1)
#endif


/*
	This file contains base class for all entities used in the application. It provides
	all interfaces required for database interaction and polymorthic access.
	Also here are stored some constant templates and enumerables which are usefull for 
	polymorthism


*/
namespace barcodeUtil
	// This namespace stores usefull but rarely used tools
{
	extern const int TOTAL_CSV_LEN; // total fields in csv string
	extern const QStringList CSV_FIELD_NAMES; // names of fields
	extern const QString CSV_STRING_TEMPLATE; // template for one string
	extern const QString CSV_HEADER_STRING; // template for header
	extern const QString CSV_BARCODE_HEADER;
	extern const QString CSV_NONBARCODE_OP_TEMPLATE; // template for two strings
	extern const QString CSV_BARCODE_STR_TEMPLATE; // template for storing a barcode
	// This enum holds barcode types which are used to define which descendant of the abs_barcode is stored in pointer
    enum
#ifdef QT_VERSION5X
            class
#endif
    barcodetypes {
		// binds entities types to barcodes
		abstractBc, uniformBc, pricedBc, shortBc , separatorNotBC};
	enum barcodeenumerables { 
		// binds roles for easier access, deprecated
		quantity, generalPrice, discountPrice };
    inline uint qHash(barcodeUtil::barcodeenumerables a)
    {
        return uint(a);
    }
    inline uint qHash(barcodeUtil::barcodetypes a)
    {
        return uint(a);
    }

}

class QSqlQuery;
class AbsEntity
{
protected:
	// must return values ready for insertion (%values%)
	virtual QString _toSql() const = 0;
	// must return table associated with this entity
	virtual const TemplatedTableHandler* _assocTable() const = 0;
	// must return short description
	virtual QString _formatedView(QString sep, QString dform) const = 0;
	// must return full description
	virtual QString _maximumInfoView(QString sep, QString dform) const = 0;
	// must return filled CSV_BARCODE_STR_TEMPLATE
	virtual QString _normalizedCsvView() const = 0;
	// must return validity for this entity
	virtual bool _isValid() const = 0;
	// must parse sql query and obtain value. Must call .next()
	virtual bool _fromSql(QSqlQuery*) = 0;
	// clones entity. Warning - id will be same. 
	virtual AbsEntity* _clone() const = 0;
	// performs deep comparation with upcast
	virtual bool deepCompare(AbsEntity* bc) const;
	// must return string which is used for identifying this entity (not id)
	virtual QString _getName() const = 0;
	// must return enumerable described by role. If role is out of range, must return 0
	virtual double _getEnumerable(int role) const = 0;
	// must set enumerable described by role. By default does nothing
	virtual void _setEnumerable(int role, double value);
	// must erase all data from entity
	virtual void _invalidate() = 0;
	// must return height of description. By default returns 2
	virtual int _getHeight() const;
	// returns field list used for table template
	virtual const QStringList& _getFields() const = 0;
	//must return comparation query for this entity. Deprecated
	virtual QString _fullComparationQuery() const = 0;
	// describes which particular entity is this
	int mytype;
	// used for database interactions
public:
	long long int GUID;
	// creates entity with new guid
	explicit AbsEntity(int type);
	// creates entity with other guid type
	explicit AbsEntity(int type, long long int guid);

	// outer interfaces for virtual methods
	const TemplatedTableHandler* getAssociatedTable() const;
	QString asSqlInsertion() const;
	QString maximizedView(QString sep, QString dform) const;
	QString formatedView(QString sep, QString dform) const;
	QString normalizedCsvView() const;
	QString fullComparationQuery() const;
	bool isValid() const;
	bool operator==(AbsEntity* bc) const;
	bool operator==(QSharedPointer<AbsEntity> bc) const;
	int myType() const;
	bool fromSql(QSqlQuery*);
	AbsEntity* clone() const;
	QString getName() const;
	double getEnumerable(int role = -1) const;
	void setEnumerable(int role = -1, double value = 0);
	void invalidate();
	int getHeight() const;
	const QStringList& getFields() const;
	static long long int makeGUID();
	long long int getId() const;
	QString serializeId() const;
	// must be virtual for correct polymorhic cleaning
    virtual ~AbsEntity(){}
};

// do not use non-typedeffed pointers
typedef QSharedPointer<AbsEntity> Entity;
typedef QVector<Entity> EntityList;

// can be used via QVariant
Q_DECLARE_METATYPE(Entity);

// used for storing dates and datetimes in database.
extern const QString datetimeDBEncoding;
extern const QString dateDBEncoding;
extern const QString timeDBEncoding;