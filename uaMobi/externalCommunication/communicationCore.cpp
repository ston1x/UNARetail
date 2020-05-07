#include "communicationCore.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QHostInfo>
#include "dataFormats/formats.h"



communicationCore::communicationCore(QObject* parent)
	: QObject(parent), deviceName(), manager(new QNetworkAccessManager(this)),
	timeoutInterval(60000)
{

}
communicationCore* communicationCore::_instanse = Q_NULLPTR;
communicationCore* communicationCore::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new communicationCore();
	}
	return _instanse;
}

QNetworkReply* communicationCore::get(QString url)
{
	return manager->get(QNetworkRequest(url));
}

QNetworkReply* communicationCore::get(QUrl url)
{
	return manager->get(QNetworkRequest(url));
}

QNetworkReply* communicationCore::post(QNetworkRequest& request, QByteArray& data)
{
	return manager->post(request, data);
}

QNetworkReply* communicationCore::sendGETRequest(QString url)
{
	return instanse()->get(url);
}

QNetworkReply* communicationCore::sendGETRequest(QUrl url)
{
	return instanse()->get(url);
}

QNetworkReply* communicationCore::sendPOSTRequest(QNetworkRequest& request, QByteArray& data)
{
	return instanse()->post(request, data);
}
