#include "GlobalAppSettings.h"
#include <QtCore/qsettings.h>
#include <QApplication>
#include <QVector>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "client_specific_defaults.h"

const float VERSION = 0.836f;
#ifdef LINELLA
const char* SUFFIX = "linella_release";
#else
const char* SUFFIX = "nigthly";
#endif

static ModeDescription defaultMode;

GlobalAppSettings::GlobalAppSettings()
{
#ifdef DEBUG
	detrace_METHCALL("initializeGlobalAppSettings");
#endif
	QSettings settings("uaMobi.ini", QSettings::IniFormat);
	initialize_default_file(settings);
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
	placeAsItem = settings.value("placeAsItem", QVariant()).toString();
	placeAsCode = settings.value("placeAsCode", QVariant()).toString();
	dlist.clear();
	extrasearchPrefix = settings.value("extrasearchPrefix", QString()).toString();
	clearScanBuffer = settings.value("clearScanBuffer", QVariant(false)).toBool();
	sendLogin = settings.value("sendLogin", QVariant(false)).toBool();
	userLogin = settings.value("userLogin", QVariant()).toString();
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
	autoFillQuantity = settings.value("autoFillQuantity", QVariant(false)).toBool();
	netEncoding = settings.value("netEncoding", QVariant("CP1251")).toByteArray();
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
	QStringList a;
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
	settings.setValue("sysfeed", a);
	a.clear();
	settings.setValue("floatControl", a);
	settings.setValue("extrasearchPrefix", extrasearchPrefix);
	settings.setValue("clearScanBuffer", clearScanBuffer);
	settings.setValue("sendLogin", sendLogin);
	settings.setValue("userLogin", userLogin);
	QStringList serializedModes;
	for (int i = 0; i < modes.count(); ++i)
	{
		serializedModes << modes.at(i).serialize();
	}
	settings.setValue("modesSettings", serializedModes);
	settings.setValue("autoFillQuantity", autoFillQuantity);
	settings.setValue("netEncoding", netEncoding);
}

ModeDescription& GlobalAppSettings::getModeDescription(Modes m)
{
	if (int(m) >= 0 && int(m) < modes.count())
	{
		return modes[int(m)];
	}
#ifdef DEBUG
	detrace_FAIL;
	detrace_METHPERROR("GlobalAppSettings::getModeDescription", "Attempt to get inexistent mode on index " << int(m));
#endif
	return defaultMode;
}

ModeDescription& GlobalAppSettings::getModeDescription(int m)
{
	return (*this)[m];
}


const QByteArray& GlobalAppSettings::getNetworkEncoding()
{
	return netEncoding;
}

void GlobalAppSettings::setNetworkEncoding(const QString& enc)
{
	netEncoding = enc.toLocal8Bit();
}

ModeDescription& GlobalAppSettings::operator[](Modes m)
{
	return getModeDescription(m);
}

ModeDescription& GlobalAppSettings::operator[](int m)
{
	if (m >= 0 && m < modes.count())
	{
		return modes[m];
	}
#ifdef DEBUG
	detrace_FAIL;
	detrace_METHPERROR("GlobalAppSettings::getModeDescription", "Attempt to get inexistent mode on index " << m);
#endif
	return defaultMode;
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
