#include "GlobalAppSettings.h"
#include <QtCore/qsettings.h>
#include <QApplication>
#include <QVector>
#ifdef DEBUG
#include "debugtrace.h"
#endif

const float VERSION = 0.79f;
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
	showHistory = settings.value("showHistory", QVariant(false)).toBool();
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
	separatorCode = settings.value("separatorCode", QVariant(QChar(','))).toChar();
	QStringList dlist = settings.value("deserializationOrder", QStringList()).toStringList();
	for (int i = 0; i < dlist.count(); ++i)
	{
		deserializationOrder.push_back(dlist.at(i).toInt());
	}
	dlist.clear();
	dlist = settings.value("deserializationPoints", QStringList()).toStringList();
	for (int i = 0; i < dlist.count(); ++i)
	{
		deserializationPoints.push_back(dlist.at(i).toInt());
	}
	dlist.clear();
	dlist = settings.value("floatControl", QVariant()).toStringList();
	if (dlist.count() != MODES_TOTAL)
	{
		floatControl << false << false << false << false << true << false;
	}
	else
	{
		QString b;
		for (int i = 0; i < MODES_TOTAL; ++i)
		{
			b = dlist.at(i);
			if (b.isEmpty())
			{
				floatControl << false;
			}
			else
			{
				floatControl << ((b.at(0) == 't'));
			}
		}
	}
	placeAsItem = settings.value("placeAsItem", QVariant()).toString();
	placeAsCode = settings.value("placeAsCode", QVariant()).toString();
	dlist.clear();
	dlist = settings.value("sysfeed", QStringList()).toStringList();
	for (int i = 0; i < dlist.count(); ++i)
	{
		sysfeed.push_back(dlist.at(i).toInt());
	}
	while (sysfeed.count() < MODES_TOTAL)
		sysfeed.push_back(-1);
	extrasearchPrefix = settings.value("extrasearchPrefix", QString()).toString();
	clearScanBuffer = settings.value("clearScanBuffer", QVariant(false)).toBool();
	sendLogin = settings.value("sendLogin", QVariant(false)).toBool();
	userLogin = settings.value("userLogin", QVariant()).toString();
	userPass = settings.value("userPass", QVariant()).toString();
	modes.resize(MODES_TOTAL);
	QStringList modesSerialized = settings.value("modesSettings", QStringList()).toStringList();
	for (int i = 0; i < MODES_TOTAL; ++i)
	{
		if (i >= modesSerialized.count())
		{
			modes << ModeDescription(modeFromInt(i));
		}
		else
		{
			modes << ModeDescription();
			if (!modes.last().deserialize(modesSerialized.at(i)))
				modes.last() = ModeDescription(modeFromInt(i));
		}
	}
	SetTranslator();
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
    QStringList a = serializeLists<int>(serializationOrder);
	settings.setValue("serializationOrder", a);
	settings.setValue("fontMaxHeight", fontMaxHeight);
	settings.setValue("fontMinHeight", fontMinHeight);
	settings.setValue("fontPercent", fontPercent);
	settings.setValue("showHistory", showHistory);
	settings.setValue("separatorCode", separatorCode);
	a.clear();
	for (int i = 0; i < deserializationOrder.count(); ++i)
	{
		a << QString::number(deserializationOrder.at(i));
	}
	settings.setValue("deserializationOrder", a);
	a.clear();
	for (int i = 0; i < deserializationPoints.count(); ++i)
	{
		a << QString::number(deserializationPoints.at(i));
	}
	settings.setValue("deserializationPoints", a);
	settings.setValue("placeAsItem", placeAsItem);
	settings.setValue("placeAsCode", placeAsCode);
	a.clear();
	for (int i = 0; i < sysfeed.count(); ++i)
	{
		a <<QString::number(sysfeed.at(i));
	}
	settings.setValue("sysfeed", a);
	a.clear();
	for (int i = 0; i < floatControl.count(); ++i)
	{
		a <<( (floatControl.at(i)) ? "true" : "false");
	}

	settings.setValue("floatControl", a);
	settings.setValue("extrasearchPrefix", extrasearchPrefix);
	settings.setValue("clearScanBuffer", clearScanBuffer);
	settings.setValue("sendLogin", sendLogin);
	settings.setValue("userLogin", userLogin);
	settings.setValue("userPass", userPass);
	QStringList serializedModes;
	for (int i = 0; i < modes.count(); ++i)
	{
		serializedModes << modes.at(i).serialize();
	}
	settings.setValue("modesSettings", serializedModes);
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
