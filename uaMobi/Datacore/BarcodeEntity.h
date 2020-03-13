#pragma once
#include "AbsEntity.h"
#include <QDateTime>



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
	virtual int _getEnumerable(int role) const override;
	virtual void _invalidate() override;
	virtual int _getHeight() const override;
public:
	QString barcode;			//	String representation of barcode. Is used to distinct one barcode from another.
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	int isUploaded;
	QDateTime expDate;		//	datetime of expiration. Is needed only for sending - no modifications after user input
	QString comment;		//	comment to barcode
	double quantity;

	explicit BarcodeEntity(QString barcode = QString(),
		QDateTime adddt = QDateTime::currentDateTime(),
		int scsess = 0,
		QDateTime expdt = QDateTime::currentDateTime(),
		QString comm = "", double Qty = 0);
	static unsigned int getEnumerableFieldIndex();
};

typedef std::shared_ptr<BarcodeEntity> Barcode;
typedef QVector<Barcode> BarcodeList;