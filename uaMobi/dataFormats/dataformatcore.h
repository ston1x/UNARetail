#ifndef DATAFORMATCORE_H
#define DATAFORMATCORE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

class DataFormatCore
{
public:
    DataFormatCore();
    virtual ~DataFormatCore();

    virtual QString getFormatedSql( QSqlQuery query ) = 0;
};
#endif
