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
public:
	QString barcode;			//	String representation of barcode. Is used to distinct one barcode from another.
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	int scanningSessions;				//	int value - quantity of scans
	QDateTime expDate;		//	datetime of expiration. Is needed only for sending - no modifications after user input
	QString comment;		//	comment to barcode
	int generalPrice;
	int discountPrice;
	int secondaryPrice;
	int unnecessaryPrice;

	explicit PricedBarcodeEntity(QString barcode = QString(),
		QDateTime adddt = QDateTime::currentDateTime(),
		int scsess = 0,
		QDateTime expdt = QDateTime::currentDateTime(),
		QString comm = "", int gP = 0, int dP = 0, int sP = 0, int unP = 0);
};

typedef std::shared_ptr<PricedBarcodeEntity> PricedBarcode;
typedef QVector<PricedBarcode> PricedBcList;