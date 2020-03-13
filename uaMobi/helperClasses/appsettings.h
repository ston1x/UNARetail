#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>

class AppSettings
{
public:
    static void setSettings( QString name , QString value );
    static QVariant getSettings( QString name );
private:
    AppSettings();
};
#endif
