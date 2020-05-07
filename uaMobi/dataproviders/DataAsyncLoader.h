#pragma once
#include <QThread>
#include <QObject>
#include "Datacore/DataEntities.h"
#include "dataproviders/sqldataprovider.h"
/*
	This class is creating new database connection and loads list of entities described by mode and table
	This list will arrive via queed connection, then you can delete this object. List is passed by value, it's ok
	because it stores only pointers.
*/

class DataAsyncLoader : public QObject
{
	Q_OBJECT
public:
    DataAsyncLoader(QObject * parent = Q_NULLPTR);
public slots:
	void loadEntitiesForMode(Modes mode, Entity prototype, TableNames table);
	void loadEntitiesForMode(Modes mode, TableNames table);
	void loadAllEntitiesForMode(Modes mode); // loads both TableNames::Scanned and TableNames::Uploaded
signals:
	// emitted when load completed
	void resultReady(EntityList);
};
