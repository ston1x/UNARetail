#pragma once
#include <QString>
#include <QList>


enum
#ifdef QT_VERSION5X
	class
#endif
	Modes
{
	// This enum represents modes. Warning! All these values must be linked with entities and 
	// ready for QString convertation. Warning! Some settings arrays are hard linked to these numbers.
	Search, Inventory, Supplies, Simple, Prices, Invoices
};

extern const int MODES_TOTAL;

typedef unsigned int uint;
inline uint qHash(Modes m)
// used in QHash container
{
	return uint(m);
}
inline Modes modeFromInt(const int m)
{
	if (m > 0 && m < MODES_TOTAL)
		return static_cast<Modes>(m);
	return Modes::Search;
}
bool hasModifiableSysfeed(Modes m);
extern const QString ModePrefixes[];

enum sendingMode { sendSent, sendUnsent, sendAll };

class ModeDescription
{
private:
	Modes mode;
	int sysfeed;
	QList<int> serializationOrder;
	bool floatControl;
	long long int previousDocument;
	bool attachNewDataToPrevious;
	bool clearBeforeAttachingNewData;
	bool allowInsertingTaxInvoiceNumber;
	bool newBCMustHaveTaxInvoiceNumber;
protected:
	virtual bool _deserialize(const QString&);
	virtual QString _serialize() const;
public:
	explicit ModeDescription();
	explicit ModeDescription(Modes md);
	explicit ModeDescription(QString& serialized);
	ModeDescription(Modes m, int sf, QList<int>& sO, bool fc,long long int pvd, bool andtp, bool cband,
	bool aITIN,
	bool NBMHTIN
	);
	bool deserialize(const QString&);
	QString serialize() const;
	Modes getMode() const;
	int getSysfeed() const;
	const QList<int>& getSerializationOrder() const;
	bool requiresFloatControl() const;
	bool requiresAttachingToPreviousDoc() const;
	bool mustClearBeforeAttaching() const;
	long long int getPreviousDocNumber() const;
	bool isInsertingTaxInvoiceNumAllowed() const;
	bool isForbiddenInsertingWithoutTaxInvoice() const;


	void setSysfeed(int);
	void setSerializationOrder(QList<int>&);
	void setFloatControl(bool);
	void setAttachingToPrevDoc(bool);
	void setCleanBeforeAttaching(bool);
	void setPreviousDocument(long long int);
	void setInsertingTaxNumber(bool);
	void setForbiddingInsertingWithoutTaxInvoice(bool);
};

enum
#ifdef QT_VERSION5X
	class
#endif
	TableNames
{
	Scanned,		//	Here are stored barcodes which was not uploaded
		Uploaded,		//	Here are stored barcodes marked as sent
		TempScan
};
inline uint qHash(TableNames t)
{
	return uint(t);
}
extern const QString TableSuffixes[];

enum
#ifdef QT_VERSION5X
	class
#endif
	Destinations {
	NetworkDestination, FileDestination
};
