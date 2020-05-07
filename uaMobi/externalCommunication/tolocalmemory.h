#pragma once
#include "externalCommunication/communicationCore.h"

/*
		This commcore child is masking with commcore interface to store data in file.
		Address is used as file name.

		Update:
			Fixed checkAdrress method - no empty checks
		Update:
			better addressChanged interaction: now address is modified

												Written by Ovidiu, modified by Anke
*/

class toLocalMemory : public QObject
{
	Q_OBJECT
protected:
	QByteArray uploadList;					//	this buffer is holding info inside the application
	QString toAdress;
	Modes currentmode;

	virtual bool checkAdress(QString& adress); // checks address
	virtual bool checkToAdress();
	void loadDataToSend(Modes mode, sendingMode sendmode, int format = -1);	//	calls dataprovider and obtains from there list of entries
	virtual bool checkLoadData();				//	simple check if data size > 0

public:
	toLocalMemory(Modes cmode, QObject* parent = Q_NULLPTR);
	QString sendingTo();

	bool send(sendingMode, int);	//	commcore interface
private:
	void saveToFile();			//	utility function which is saving data to file
	void applyAddressFix();

public slots:
	void addressChanged(QString address);

signals:
	void addressInvalid(QString data = QString());					//	emitted when adress is wrong
	void dataErrorOccured();				//	emitted when no data to send
	void operationDone(QString);
};
