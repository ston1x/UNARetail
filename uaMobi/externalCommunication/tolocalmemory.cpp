#include "tolocalmemory.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "dataFormats/formats.h"

bool toLocalMemory::checkAdress(QString& adress)
{
	if (adress.isEmpty())
	{
		return false;
	}
	return true;
}

bool toLocalMemory::checkToAdress()
{
	return checkAdress(toAdress);
}

void toLocalMemory::loadDataToSend(Modes mode, sendingMode sendmode, int format)
{
	uploadList.clear();
	uploadList.append(AppData->uploadFullList(mode, sendmode, tr("current_lang"), format, int(Destinations::FileDestination)));
}

bool toLocalMemory::checkLoadData()
{
	if (uploadList.isEmpty())
	{
		emit dataErrorOccured();
		return false;
	}
	return true;
}

toLocalMemory::toLocalMemory(Modes cmode, QObject * parent)
	:QObject(parent), uploadList(), toAdress(),currentmode(cmode)
{
	//detrace_DCONSTR("toLocalMemory")

#ifdef Q_OS_WIN32
	toAdress = "D:/";
#endif
	applyAddressFix();
}
QString toLocalMemory::sendingTo()
{
	return toAdress;
}

bool toLocalMemory::send(sendingMode mode, int format)
{

		loadDataToSend(currentmode, mode, format);


		if (!checkLoadData())
        {

				return false;
		};

	if (!checkToAdress())
    {

			return false;
	};

	saveToFile();
	return true;
}


void toLocalMemory::saveToFile()
//	This method is saving data to text file on sendingTo() path
{

	if (!sendingTo().contains("UamobiExport"))
	{
		applyAddressFix();
	}
	QFile file(sendingTo());

	file.remove();

	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);
		stream << uploadList;
        stream.flush();
        file.close();
		emit operationDone("true");
	}
	else
    {

			emit operationDone("false");
	}
}

void toLocalMemory::applyAddressFix()
{
	if (!(toAdress.endsWith(".txt") || toAdress.endsWith(".xml")
		|| toAdress.endsWith(".json" || toAdress.endsWith(".csv"))))
	{
		toAdress += "/UamobiExport_" + QString::number(int(currentmode))
			+ "_" + QDate::currentDate().toString() + ".txt";
	}
}

void toLocalMemory::addressChanged(QString address)
{
		toAdress = address + "/UamobiExport_" + QString::number(int(currentmode))
			+ "_" + QDate::currentDate().toString() + ".txt";
		applyAddressFix();
}
