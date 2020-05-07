#pragma once
#include "AbsEntity.h"
#include <QDateTime>

/*
	this entity describes default barcode type used for most modes - barcode and it's quantity and 
	supportive values.
*/



class BarcodeEntity : public AbsEntity
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
	virtual bool deepCompare(AbsEntity*) const override;
	virtual QString _getName() const override;
	virtual double _getEnumerable(int role) const override;
	virtual void _invalidate() override;
	virtual int _getHeight() const override;
	virtual const QStringList& _getFields() const override;
	virtual QString _fullComparationQuery() const override;
	virtual void _setEnumerable(int role, double value) override;
public:
	QString barcode;			//	String representation of barcode. Is used to distinct one barcode from another.
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	int isUploaded;
	QDateTime expDate;		//	datetime of expiration. Is needed only for sending - no modifications after user input
	QString comment;		//	comment to barcode
	double quantity;

	explicit BarcodeEntity(QString barcode = QString(),
		QDateTime adddt = QDateTime::currentDateTime(),
		int isUpl = 0,
		QDateTime expdt = QDateTime::currentDateTime(),
		QString comm = "", double Qty = 0);
	explicit BarcodeEntity(QString bc, QString comm);
	static unsigned int getEnumerableFieldIndex();
};

typedef QSharedPointer<BarcodeEntity> Barcode;
typedef QVector<Barcode> BarcodeList;