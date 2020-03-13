#include "communicationCore.h"
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QHostInfo>

communicationCore::communicationCore(Modes cmode)
	: currentmode(cmode)
{
	getName();
}

void communicationCore::getName()
{
#ifdef Q_OS_WIN32
	deviceName = QHostInfo::localHostName();
#endif
#ifdef Q_OS_ANDROID
	deviceName = "Android device" + QString::number(rand());
#endif
}

bool communicationCore::checkAdress(QString& adress)
{
	if (adress == "")
	{
		emit addressInvalid();
		return false;
	}
	return true;
}

bool communicationCore::checkFromAdress()
{
	return checkAdress(fromAdress);
}

bool communicationCore::checkToAdress()
{
	return checkAdress(toAdress);
}

void communicationCore::loadDataToSend(Modes mode, sendingMode sendmode, int format)
{
	uploadList.clear();
	uploadList.append(AppData->uploadFullList(mode, sendmode, tr("current_lang"), format).toStdString().c_str());
}

bool communicationCore::checkLoadData()
{
	if (uploadList.isEmpty())
	{
		emit dataErrorOccured();
		return false;
	}
	return true;
}

QString communicationCore::sendingTo()
{
	return toAdress;
}

void communicationCore::addressChanged(QString newAddress, bool isFrom)
{
	if (isFrom)
	{
		fromAdress = newAddress;
	}
	else
	{
		toAdress = newAddress;
	}
}

QString communicationCore::gettingFrom()
{
	return fromAdress;
}