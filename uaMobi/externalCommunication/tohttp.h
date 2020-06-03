#pragma once

#include "externalCommunication/communicationCore.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QLinkedList>

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
class QTextDecoder;
class QTextCodec;
class toHttp : public QObject
{
	Q_OBJECT
private:
	QString address;
	Modes currentMode;
	QNetworkReply* awaitedReply;					//	used for awaiting data

	long long int li, lip;						//	these are currentlyRead value and totalLength value
	QString input;							//	here is stored input

	QLinkedList<ShortBarcode> loaded;

	int currentlyAwaiting;
	QTextDecoder * decoder;
	int page;
	int sessionCounter;
	QString stored_place_code;
	enum {
		notAwaiting,
		awaitingPlaces,
		awaitingProducts
	};
	QPair<QStringList, QStringList> _parsePlaceList();
	QString _loadDataToSend(sendingMode sendmode, int format);
	void _parseProductList();
	bool _checkAddress();
	bool _setReply(QNetworkReply*);
	bool _product_list_receiving_start();
	bool _product_list_receiving_block(QString);
	bool _product_list_receiving_end();
	bool _send_get_products();
public:
	toHttp(Modes mode);
	~toHttp();
	void setAddress(QString newAddress);

	bool send(sendingMode, int); // interface override
	bool isUsed() const { return currentlyAwaiting != notAwaiting; };
	void dropAwaiting();
	bool getPlacelist();
	bool getProductList( QString place_code);
	void clear();
	int count();
private slots:
	void uploadResponce();	//	is triggered when upload responce came
	void downloadResponce();	//	receives downoad response and stores it in input

signals:
	void readNL();									//	emitted when block is partially arrived
	void progressLeap(int);							//	emmited when part of the process is passed

	void downloadStateChanged(QString state);
	void placelistReceived(QStringList names, QStringList codes);
	void errorReceived(QString stack = QString(),QString message = QString(), int code = 404);
	void sendSuccesfull(QString);
};
