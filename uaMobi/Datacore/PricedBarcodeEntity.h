#pragma once
#include "AbsEntity.h"
#include <QtCore/QDateTime>

class PricedBarcodeEntity : public AbsEntity
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
	virtual int _getEnumerable(int role) const override;
	virtual bool deepCompare(AbsEntity*) const override;
	virtual void _invalidate() override;
	virtual int _getHeight() const override;
	virtual const QStringList& _getFields() const override;
public:
	QString barcode;			//	String representation of barcode. Is used to distinct one barcode from another.
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	QDateTime expDate;		//	datetime of expiration. Is needed only for sending - no modifications after user input
	QString comment;		//	comment to barcode
	double generalPrice;
	double discountPrice;
	double secondaryPrice;
	double unnecessaryPrice;

	explicit PricedBarcodeEntity(QString barcode = QString(),
		QDateTime adddt = QDateTime::currentDateTime(),
		QDateTime expdt = QDateTime::currentDateTime(),
		QString comm = "", double gP = 0, double dP = 0, double sP = 0, double unP = 0);
};

typedef std::shared_ptr<PricedBarcodeEntity> PricedBarcode;
typedef QVector<PricedBarcode> PricedBcList;