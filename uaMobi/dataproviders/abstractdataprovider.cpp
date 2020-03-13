#include "abstractdataprovider.h"

AbstractDataProvider::AbstractDataProvider(QObject *parent) :
    QObject(parent)
{
}

QString AbstractDataProvider::getInfo(QString code)
{
    return QString();
}

void AbstractDataProvider::postData(QString code, double data)
{
}

double AbstractDataProvider::getAccumulatedQty(QString code)
{
    return 0;
}
