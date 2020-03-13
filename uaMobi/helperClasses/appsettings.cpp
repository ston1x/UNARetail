#include "appsettings.h"

AppSettings::AppSettings()
{
}

void AppSettings::setSettings( QString name , QString value )
{
    QSettings set{ "settings.ini" , QSettings::IniFormat };
    return set.setValue( name , value );
}

QVariant AppSettings::getSettings( QString name )
{
    QSettings set{ "settings.ini" , QSettings::IniFormat };
    return set.value( name );
}
