#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QObject>

class AbstractDataProvider : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDataProvider(QObject *parent = 0);
    virtual QString getInfo(QString code);
    virtual double getAccumulatedQty(QString code);
    virtual void postData(QString code, double data);

signals:

public slots:

};

#endif // DATAPROVIDER_H
