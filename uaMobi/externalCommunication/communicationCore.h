#pragma once
#include <QObject>
#include "dataproviders/sqldataprovider.h"

/*
			This class is abstract basic for all protocol-based communications. It holds
		basic connection info (addresses, database connection, byte buffer) and provides
		interface for it's children.

			pure virtual send\get pair are main methods of this class. Both are responsible for
		sending full scanned table, handling everything themselves.
			protected functions are providing utility fuctions - as address checking, data checking,
		data preparing, receiving name.
			single slot is oriented to change adresses when they are changed outside of this class.
		bool variable defines is this address FROM.
			signals are providing outer interface for classes. This interface allows users of commcore-based
		classes to catch minor connection errors and operation status

		minor update: technical fixes linked to global changes in barcode handling

																			Written by Ovidiu, modified by Anke
*/

class communicationCore : public QObject	//	Child of QObject to use signals
{
	Q_OBJECT
protected:
	QString deviceName;						//	obtained by getName()
	QByteArray uploadList;					//	this buffer is holding info inside the application
	QString fromAdress;						//	Maybe QUrl is better?
	QString toAdress;
	Modes currentmode;

	void getName();							//	sets deviceName

	virtual bool checkAdress(QString& adress); // checks address
	virtual bool checkFromAdress();
	virtual bool checkToAdress();
	void loadDataToSend(Modes mode, sendingMode sendmode, int format = -1);	//	calls dataprovider and obtains from there list of entries
	virtual bool checkLoadData();				//	simple check if data size > 0

public:
	communicationCore(Modes cmode); // possibly parent structure will be better

	virtual bool send(sendingMode, int format = -1) = 0;				//	pure virtual to avoid using naked commcore class
	virtual void get() = 0;

	QString gettingFrom();				//	getters for addresses
	QString sendingTo();

public slots:
	virtual void addressChanged(QString address, bool isFrom);		//	changes addresses

signals:
	void addressInvalid();					//	emitted when adress is wrong
	void dataErrorOccured();				//	emitted when no data to send
	void operationDone(bool);				//	emitted when operation ends despite it's success
};
