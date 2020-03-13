#pragma once

#include "externalCommunication/communicationCore.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


/*
	This child of commcore is creating http-connection for implementation of send\get pair
	It is creating networkAccessManager and NetworkReply for networking and some variables for
	storing input and relation with currently read and data in buffer.
	It adds private slots for listening responce and signal for outer application:
	progressLeap is emitted when another block of data is read. It is holding % of read data.

	Update:
		lot of detracing added

																Written by Ovidiu, modified by Anke
*/
class toHttp : public communicationCore		//	Child of commcore oriented for http protocol
{
	Q_OBJECT
private:
	QNetworkAccessManager* manager;			//	used for establishing connection
	QNetworkReply* gReplay;					//	used for awaiting data

	long long int li, lip;						//	these are currentlyRead value and totalLength value
	QString input;							//	here is stored input
	bool inProgress;
	QObject* networkManager;				//	This is link to dangling manager. Is this a good decision?

public:
	toHttp(Modes mode);

	virtual bool send(sendingMode, int) override; // interface override
	virtual void get() override;
	bool isUsed() const { return inProgress; };
	void dropAwaiting();
private slots:
	void uploadResponce(QNetworkReply* replay);	//	is triggered when upload responce came
	void downloadResponce(QNetworkReply* replay);	//	receives downoad response and stores it in input
	void readingNL();								//	activated when block is partially arrived

signals:
	void readNL();									//	emitted when block is partially arrived
	void progressLeap(int);							//	emmited when part of the process is passed
};
