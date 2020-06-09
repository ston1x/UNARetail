#pragma once
#define LINELLA
#ifdef LINELLA
#include <QSettings>
#include "dataproviders/ModesDescriptions.h"
#include <QString>
#include <QVariant>

inline void initialize_default_file(QSettings& settings)
{
	if (!settings.allKeys().isEmpty())
		return;
	// settings default for all linella users
	settings.setValue("httpAddressIn", QUrl::fromUserInput("http://92.115.237.226:60080/um/prices_uniback.php"));
	settings.setValue("language", "Romanian");
	settings.setValue("autoSearch", QVariant(true));
	settings.setValue("additionalElements", true);
	settings.setValue("clearScanBuffer", QVariant(true));

	QList<ModeDescription> modes;
	for (int i = 0; i < MODES_TOTAL; ++i)
	{
		modes << ModeDescription(modeFromInt(i));
	}
	modes[int(Modes::Prices)].setAttachingToPrevDoc(true);
	QStringList serializedModes;
	for (int i = 0; i < modes.count(); ++i)
	{
		serializedModes << modes.at(i).serialize();
	}
	settings.setValue("modesSettings", serializedModes);
}
#else
inline void initialize_default_file()
{
	return;
}
#endif