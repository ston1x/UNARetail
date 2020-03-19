#include "GlobalAppSettings.h"
#include <QtCore/qsettings.h>
#include <QtWidgets/QApplication>
#include <QVector>
#ifdef DEBUG
#include "debugtrace.h"
#endif

const float VERSION = 0.72f;
const char* SUFFIX = "nigthly";



GlobalAppSettings::GlobalAppSettings()
{
#ifdef DEBUG
	detrace_METHCALL("initializeGlobalAppSettings");
#endif
	QSettings settings("uaMobi.ini", QSettings::IniFormat);
#ifdef DEBUG
	detrace_METHEXPL("if errors: " << settings.status());
#endif
#ifdef Q_OS_WIN
	localfile = settings.value("localAddress", "C:/").toString();
#endif // Q_OS_WIN
#ifdef Q_OS_ANDROID
	localfile = settings.value("localAddress", "/storage/emulated/0/").toString();
#endif
	httpOut = settings.value("httpAddressOut").toUrl();
	httpIn = settings.value("httpAddressIn").toUrl();
	additionalControlElements = settings.value("additionalElements").toBool();
	autoSearch = settings.value("autoSearch", QVariant(true)).toBool();
	language = settings.value("language", "English").toString();
	simpleSending = settings.value("simpleSending", false).toBool();
	sendingDirection = settings.value("sendingDirection", 0).toInt();
	sendingFormat = settings.value("sendingFormat", 0).toInt();
	scanPrefix = settings.value("scanPrefix", QVariant(int('$'))).toInt();
	scanSuffix = settings.value("scanSuffix", QVariant(int('\n'))).toInt();
	navigationElements = settings.value("navigation", QVariant(true)).toBool();
	scanButtonCode = settings.value("scanButtonCode", QVariant(int('`'))).toInt();
	localDatabase = settings.value("localDatabase", QVariant()).toString();
	fontMaxHeight = settings.value("fontMaxHeight", QVariant(30)).toInt();
	fontMinHeight = settings.value("fontMinHeight", QVariant(10)).toInt();
	fontPercent = settings.value("fontPercent", QVariant(0.03)).toDouble();
	QStringList temp = settings.value("serializationOrder", QStringList()).toStringList();
	QList<int> orderForOneMode;
	serializationOrder.reserve(temp.count());
	for (int i = 0; i < temp.count(); ++i)
	{
		QStringList splitted(temp[i].split("|", QString::SplitBehavior::SkipEmptyParts));
		for (int j = 0; j < splitted.count(); ++j)
		{
			orderForOneMode << splitted[j].toInt();
		}
		serializationOrder.push_back(orderForOneMode);
	}
	if (serializationOrder.isEmpty())
	{
		serializationOrder.push_back(QList<int>());
		serializationOrder.push_back(QList<int>());
		serializationOrder.push_back(QList<int>());
		serializationOrder.push_back(QList<int>());
	}
	SetTranslator();
#ifdef DEBUG
	detrace_METHDATAS("initializeGlobalAppSettings", "localfile, httpin, httpout", << localfile << httpIn.toString() << httpOut.toString());
#endif
}

void GlobalAppSettings::SetTranslator()
{
	if (language == "Russian")
	{
		qt_translator.load(":/translations/unaretail_ru.qm", ".");
	}
	else if (language == "Romanian")
	{
		qt_translator.load(":/translations/unaretail_ro.qm", ".");
	}
	else
	{
		qt_translator.load(":/translations/unaretail_en.qm", ".");
	}
	qApp->installTranslator(&qt_translator);
}

void GlobalAppSettings::Save()
{
#ifdef DEBUG
	detrace_METHCALL("dumpGlobalAppSettings");
#endif
	QSettings settings("uaMobi.ini", QSettings::IniFormat);
#ifdef DEBUG
	detrace_METHEXPL("if errors: " << settings.status());
#endif
	settings.setValue("localAddress", localfile);
	settings.setValue("httpAddressOut", httpOut);
	settings.setValue("httpAddressIn", httpIn);
	settings.setValue("additionalElements", additionalControlElements);
	settings.setValue("language", language);
	settings.setValue("autoSearch", autoSearch);
	settings.setValue("simpleSending", simpleSending);
	settings.setValue("sendingDirection", sendingDirection);
	settings.setValue("sendingFormat", sendingFormat);
	settings.setValue("scanPrefix", scanPrefix);
	settings.setValue("scanSuffix", scanSuffix);
	settings.setValue("scanButtonCode", scanButtonCode);
	settings.setValue("navigation", navigationElements);
	settings.setValue("localDatabase", localDatabase);
	auto a = serializeLists<int>(serializationOrder);
	settings.setValue("serializationOrder", serializeLists<int>(serializationOrder));
	settings.setValue("fontMaxHeight", fontMaxHeight);
	settings.setValue("fontMinHeight", fontMinHeight);
	settings.setValue("fontPercent", fontPercent);
#ifdef DEBUG
	detrace_METHDATAS("dumpGlobalAppSettings", "localfile, httpIn, httpOut", << localfile << httpIn.toString() << httpOut.toString());
#endif
}

GlobalAppSettings::~GlobalAppSettings()
{
	Save();
}

GlobalAppSettings* GlobalAppSettings::_instanse = Q_NULLPTR;
GlobalAppSettings* GlobalAppSettings::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new GlobalAppSettings();
	}
	return _instanse;
}
