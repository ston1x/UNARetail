#pragma once
#include "AbsEntity.h"



class ShortBarcodeEntity : public AbsEntity
{
protected:
	// Inherited via AbsEntity
	virtual QString _toSql() const override;
	virtual const TemplatedTableHandler* _assocTable() const override;
	virtual QString _formatedView(QString sep, QString dform) const override;
	virtual QString _maximumInfoView(QString sep, QString dform) const override;
	virtual QString _normalizedCsvView() const override;
	virtual bool _isValid() const override;
	virtual bool _fromSql(QSqlQuery*) override;
	virtual AbsEntity* _clone() const override;
	virtual QString _getName() const override;
	virtual double _getEnumerable(int role) const override;
	virtual bool deepCompare(AbsEntity*) const override;
	virtual void _invalidate() override;
	virtual const QStringList& _getFields() const override;
	virtual QString _fullComparationQuery() const override;
public:
	QString barcode;			//	String representation of barcode. Is used to distinct one barcode from another.
	int code;		
	QString info;
	QString count;

	explicit ShortBarcodeEntity(QString barcode = QString(),
		int cod = 0,
		QString info = QString(), QString count = QString());

	void clear();

	static const TemplatedTableHandler* getTableHandler();
	static const QString makeInsertionFromString(QString);
	static const QStringList & getFieldNames();
	static QSharedPointer<ShortBarcodeEntity> extractFromLine(QString line);
};

typedef QSharedPointer<ShortBarcodeEntity> ShortBarcode;
typedef QVector<ShortBarcode> ShortBcList;