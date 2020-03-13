#include "tolocalmemory.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#ifdef DEBUG
#include "debugtrace.h"
#endif

toLocalMemory::toLocalMemory(Modes cmode)
	: communicationCore(cmode)
{
	//detrace_DCONSTR("toLocalMemory")
	// Android black magic to make output path

#ifdef Q_OS_WIN32
	toAdress = "D:/";
#endif
	applyAddressFix();
}

bool toLocalMemory::send(sendingMode mode, int format)
{
#ifdef DEBUG

	detrace_METHCALL("toLocalMemory::send")
		detrace_METHINVOK("toLocalMemory", "send", "loadDataToSend", "commcore")
#endif

		loadDataToSend(currentmode, mode, format);

#ifdef DEBUG
	detrace_METHINVOK("toLocalMemory", "send", "checkLoadData", "commcore")
#endif

		if (!checkLoadData())
		{
#ifdef DEBUG
			detrace_METHPERROR("toLocalMemory::send", "load data failed")
#endif

				return false;
		};

	if (!checkToAdress())
	{
#ifdef DEBUG
		detrace_METHPERROR("toLocalmemory::send", "checkToAddress failed")
#endif

			return false;
	};

	saveToFile();
	return true;
}

void toLocalMemory::get()
{
	//detrace_METHCALL("toLocalMemory::get")
	emit operationDone(false);
}

void toLocalMemory::saveToFile()
//	This method is saving data to text file on sendingTo() path
{
#ifdef DEBUG
	detrace_METHCALL("toLocalMemory::saveToFile");
#endif

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
		emit operationDone(true);
	}
	else
	{
#ifdef DEBUG
		detrace_METHPERROR("toLocalMemory::saveTofile", "file was not open")
#endif

			emit operationDone(false);
	}
}

void toLocalMemory::applyAddressFix()
{
	if (!(toAdress.endsWith(".txt") || toAdress.endsWith(".xml")
		|| toAdress.endsWith(".json" || toAdress.endsWith(".csv"))))
	{
		toAdress += "UamobiExport_" + QString::number(int(currentmode))
			+ "_" + QDate::currentDate().toString() + ".txt";
	}
}

void toLocalMemory::addressChanged(QString address, bool isFrom)
{
	if (!isFrom) {
		toAdress = address + "UamobiExport_" + QString::number(int(currentmode))
			+ "_" + QDate::currentDate().toString() + ".txt";
		applyAddressFix();
	}
}