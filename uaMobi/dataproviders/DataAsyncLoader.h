#pragma once
#include <QThread>
#include <QObject>
#include "Datacore/DataEntities.h"
#include "dataproviders/sqldataprovider.h"


class DataAsyncLoader : public QObject
{
	Q_OBJECT
public:
	DataAsyncLoader(QObject * parent);
public slots:
	void loadEntitiesForMode(Modes mode, Entity prototype, TableNames table);
	void loadEntitiesForMode(Modes mode, TableNames table);
	void loadAllEntitiesForMode(Modes mode);
signals:
	void resultReady(EntityList);
};