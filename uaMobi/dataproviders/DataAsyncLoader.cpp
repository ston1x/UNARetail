#include "DataAsyncLoader.h"

DataAsyncLoader::DataAsyncLoader(QObject * parent)
	: QObject(parent)
{
}

void DataAsyncLoader::loadEntitiesForMode(Modes mode, Entity prototype, TableNames table)
{
	QSqlDatabase tempConnection = AppData->cloneConnection();
	QString formatedName = formatTableName(mode, prototype, table);
	tempConnection.open();
	QSqlQuery q(tempConnection);
	q.exec(
		prototype->getAssociatedTable()->select_all(formatedName)
	);
	EntityList Entities;
	while (prototype->fromSql(&q))
	{
		Entities.push_back(Entity(prototype->clone()));
	}
	emit resultReady(Entities);
}

void DataAsyncLoader::loadEntitiesForMode(Modes mode, TableNames table)
{
	loadEntitiesForMode(mode, Entity(modenamesLinker[mode]->clone()), table);
}

void DataAsyncLoader::loadAllEntitiesForMode(Modes mode)
{
    QSqlDatabase tempConnection = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("maindbAsLoader" ) + QString::number(QTime::currentTime().msec()));
    tempConnection.setDatabaseName(QStringLiteral("MAIN.db"));
	Entity prototype = modenamesLinker.value(mode);
	QString formatedName = formatTableName(mode, prototype, TableNames::Scanned);
	tempConnection.open();
	QSqlQuery q(tempConnection);
	q.exec(
		prototype->getAssociatedTable()->select_all(formatedName)
	);
	EntityList Entities;
	Entities.reserve(300);
	while (prototype->fromSql(&q))
	{
		Entities.push_back(Entity(prototype->clone()));
	}
	Entities.push_back(Entity(new SeparatorEntity()));
	formatedName = formatTableName(mode, prototype, TableNames::Uploaded);
	q.exec(
		prototype->getAssociatedTable()->select_all(formatedName)
	);
	while (prototype->fromSql(&q))
	{
		Entities.push_back(Entity(prototype->clone()));
	}
    tempConnection.close();
	emit resultReady(Entities);
}
