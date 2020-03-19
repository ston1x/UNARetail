#pragma once
#include <QString>
#include "dataproviders/TableHandlers.h"
#include <QVector>
#include <qmetatype.h>

namespace barcodeUtil
	// This namespace stores usefull but rarely used tools
{
	extern const int TOTAL_CSV_LEN;
	extern const QStringList CSV_FIELD_NAMES;
	extern const QString CSV_STRING_TEMPLATE;
	extern const QString CSV_HEADER_STRING;
	extern const QString CSV_BARCODE_HEADER;
	extern const QString CSV_NONBARCODE_OP_TEMPLATE;
	extern const QString CSV_BARCODE_STR_TEMPLATE;
	// This enum holds barcode types which are used to define which descendant of the abs_barcode is stored in pointer
	enum class barcodetypes { abstractBc, uniformBc, pricedBc, shortBc };
	enum barcodeenumerables { quantity, generalPrice, discountPrice, };
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
	virtual QString _toSql() const = 0;
	virtual const TemplatedTableHandler* _assocTable() const = 0;
	virtual QString _formatedView(QString sep, QString dform) const = 0;
	virtual QString _maximumInfoView(QString sep, QString dform) const = 0;
	virtual QString _normalizedCsvView() const = 0;
	virtual bool _isValid() const = 0;
	virtual bool _fromSql(QSqlQuery*) = 0;
	virtual AbsEntity* _clone() const = 0;
	virtual bool deepCompare(AbsEntity* bc) const;
	virtual QString _getName() const = 0;
	virtual int _getEnumerable(int role) const = 0;
	virtual void _invalidate() = 0;
	virtual int _getHeight() const;
	virtual const QStringList& _getFields() const = 0;
	int mytype;

public:
	explicit AbsEntity(int type);

	const TemplatedTableHandler* getAssociatedTable() const;
	QString asSqlInsertion() const;
	QString maximizedView(QString sep, QString dform) const;
	QString formatedView(QString sep, QString dform) const;
	QString normalizedCsvView() const;
	bool isValid() const;
	bool operator==(AbsEntity* bc) const;
	bool operator==(std::shared_ptr<AbsEntity> bc) const;
	int myType() const;
	bool fromSql(QSqlQuery*);
	AbsEntity* clone() const;
	QString getName() const;
	int getEnumerable(int role = -1) const;
	void invalidate();
	int getHeight() const;
	const QStringList& getFields() const;
    virtual ~AbsEntity(){}
};

typedef std::shared_ptr<AbsEntity> Entity;
typedef QVector<Entity> EntityList;

Q_DECLARE_METATYPE(Entity);

extern const QString datetimeDBEncoding;
extern const QString dateDBEncoding;
