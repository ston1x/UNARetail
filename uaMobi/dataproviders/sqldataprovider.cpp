#include "sqldataprovider.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <qlinkedlist.h>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromUtf8("" A "", sizeof(A) - 1)
#endif
#include "widgets/utils/GlobalAppSettings.h"




QString formatTableName(Modes mode, Entity prototype, TableNames tab)
{
	return ModePrefixes[int(mode)]
		+ prototype->getAssociatedTable()->declaration() + TableSuffixes[int(tab)];
}
QHash<Modes, Entity> _initModenamesLinker()
{
	QHash<Modes, Entity> temp;
	temp.insert(Modes::Inventory, entityLinker[barcodeUtil::barcodetypes::uniformBc]);
	temp.insert(Modes::Search, entityLinker[barcodeUtil::barcodetypes::uniformBc]);
	temp.insert(Modes::Simple, entityLinker[barcodeUtil::barcodetypes::uniformBc]);
	temp.insert(Modes::Supplies, entityLinker[barcodeUtil::barcodetypes::uniformBc]);
	temp.insert(Modes::Prices, entityLinker[barcodeUtil::barcodetypes::pricedBc]);
	temp.insert(Modes::Invoices, entityLinker[barcodeUtil::barcodetypes::uniformBc]);
	return temp;
};
const QHash<Modes, Entity> modenamesLinker(_initModenamesLinker());


QString dbnameToMode(Modes dbnm)
{
	return ModePrefixes[int(dbnm)];
}

void SqlDataProvider::_assertAndOpenSession()
{
	if (isSessionOpen)
	{
		if (mainDb.isOpen())
			return;
	}
	mainDb.open();
	isSessionOpen = true;
}
void SqlDataProvider::_assertAndCloseSession()
{
	if (isSessionOpen)
	{
		mainDb.close();
		isSessionOpen = false;
	}
}

QueryPtr SqlDataProvider::runQuery(QString sql)
{
	_assertAndOpenSession();
    QueryPtr q(new QSqlQuery(mainDb));
	q->exec(sql);
#ifdef DEBUG
	detrace_METHFRECALL("execing query: " << sql);
#endif
	if (q->lastError().isValid()) {
#ifdef DEBUG
		detrace_METHPERROR("DBEXECUTE: " << sql << "\n", " : " + q->lastError().text());
#endif
        q.clear();
		_assertAndCloseSession();
		return q;
	}
	return q;
}

SqlDataProvider::SqlDataProvider()		//	Must not be called twice - ensure that exists only one SqlDataProv
{
	//detrace_DCONSTR("SqlDataProvider")
    mainDb = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("maindb.db"));
    mainDb.setDatabaseName(QStringLiteral("MAIN.db"));

	createDefaultTables();
}

bool SqlDataProvider::createTablesOf(const QList<Modes> modes, Entity proto, const QList<TableNames> tabs)		//	This method creates all tables except the downLoadedDb
//creates table from standard declarations set under indexex provided in tabs on databases provided through dbs
{
	_assertAndOpenSession();
	QString formatedTablename;
	QList<Modes>::const_iterator prefix = modes.begin();
	while (prefix != modes.end())
	{
		QList<TableNames>::const_iterator suffix = tabs.begin();
		while (suffix != tabs.end())
		{
			formatedTablename = ModePrefixes[int(*prefix)] + 
				proto->getAssociatedTable()->definition() +
				TableSuffixes[int(*suffix)];
			if (mainDb.contains())
			{
				if (!execInSession(proto->getAssociatedTable()->definition(formatedTablename)))
				{
					_assertAndCloseSession();
					return false;
				}
			}
			++prefix;
		}
		++suffix;
	}
	_assertAndCloseSession();
	return true;
}
bool SqlDataProvider::deleteEntity(Modes mode, Entity toDelete, TableNames tab)
{
	QString fname = formatTableName(mode, toDelete, tab);
	return DBexecute(toDelete->getAssociatedTable()->delete_by_primary_key(
		toDelete->serializeId(),
		fname
	));
}
bool SqlDataProvider::replaceEntityIn(Modes mode, Entity oldOne, Entity newOne, TableNames tab)
{
	QString fname = formatTableName(mode, oldOne, tab);
	DBexecute(oldOne->getAssociatedTable()->delete_by_primary_key(oldOne->serializeId(), fname));
	return DBexecute(newOne->getAssociatedTable()->insert(newOne->asSqlInsertion(), fname));
	
}
bool SqlDataProvider::wipeAll()
{
	_assertAndOpenSession();
	QList<Modes> modes;
	modes << Modes::Inventory << Modes::Simple << Modes::Supplies;
	QList<TableNames> tabs;
	tabs << TableNames::Scanned << TableNames::Uploaded;
	QList<Modes>::iterator mode = modes.begin();
	while (mode != modes.end())
	{
		QList<TableNames>::iterator tabname = tabs.begin();
		while (tabname != tabs.end())
		{
			dropTableOf(*tabname, *mode);
			createTableOf(*tabname, *mode);
			++tabname;
		}
		++mode;
	}
	QList<TableNames>::iterator tabname = tabs.begin();
	while (tabname != tabs.end())
	{
		dropTableOf(*tabname, Modes::Prices);
		createTableOf(*tabname, Modes::Prices);
		++tabname;
	}
	mainDb.commit();
	_assertAndCloseSession();
	return true;
}
bool SqlDataProvider::createDefaultTables()
// creates lists of default tables and databases and initiates it's creation
{
	_assertAndOpenSession();
    QList<Modes> modes;
    modes << Modes::Inventory << Modes::Simple << Modes::Supplies << Modes::Invoices;
    QList<TableNames> tabs;
    tabs << TableNames::Scanned << TableNames::Uploaded;
	QList<Modes>::iterator mode = modes.begin();
	while (mode != modes.end())
	{
		QList<TableNames>::iterator tabname = tabs.begin();
		while (tabname != tabs.end())
		{
			checkAndRefreshTable(*tabname, entityLinker[barcodeUtil::barcodetypes::uniformBc], *mode);
			++tabname;
		}
		++mode;
	}
	QList<TableNames>::iterator tabname = tabs.begin();
	while (tabname != tabs.end())
	{
		checkAndRefreshTable(*tabname, entityLinker[barcodeUtil::barcodetypes::pricedBc], Modes::Prices);
		++tabname;
	}
	mainDb.commit();
	_assertAndCloseSession();
	return true;
}
bool SqlDataProvider::createTableOf(TableNames tab, Entity prototype, Modes mode)
// Creates one single table under index tab on provided database
{
	_assertAndOpenSession();
	QString formatedName = formatTableName(mode, prototype, tab);
	if (!mainDb.tables().contains(formatedName))
	{
		if (!execInSession(prototype->getAssociatedTable()->definition(formatedName)))
		{
			_assertAndCloseSession();
			return false;
		}
	}
	mainDb.commit();
	_assertAndCloseSession();
	return true;
}

bool SqlDataProvider::createTableOf(TableNames tab, Modes mode)
{
	return createTableOf(tab, modenamesLinker[mode], mode);
}

bool SqlDataProvider::checkAndRefreshTable(TableNames tab, Entity prototype, Modes mode)
{
	_assertAndOpenSession();
	QString formatedName = formatTableName(mode, prototype, tab);
	if (!mainDb.tables().contains(formatedName))
	{
		createTableOf(tab, prototype, mode);
	}
	else
	{
		QueryPtr q = runQuery(
			QStringLiteral("PRAGMA TABLE_INFO(") +
			formatedName +
			QStringLiteral(")")
		);
		if (q == Q_NULLPTR)
		{
			return false;
		}
		QStringList temp;
		while (q->next())
        {
            temp << q->value(0).toString();
		}
        if (prototype->getAssociatedTable()->getFields().count() == temp.count())
			return true;
		else
		{
			dropTableOf(tab,  prototype, mode);
			createTableOf(tab, prototype, mode);
			return true;
		}
	}
	return true;
}

bool SqlDataProvider::checkAndRefreshTable(TableNames tab, Modes mode)
{
	return checkAndRefreshTable(tab, modenamesLinker[mode], mode);
}

bool SqlDataProvider::pushEntityList(EntityList& scanned, Modes mode, TableNames tab)
//This method is saving barcodes using other methods
{
	if (scanned.isEmpty())
		return false;
	_assertAndOpenSession();
	EntityList::iterator start = scanned.begin();
	while (start != scanned.end())
	{
		if (!postEntityInto(tab, *start, mode))
		{
			_assertAndCloseSession();
			return false;
		}
		++start;
	}
	_assertAndCloseSession();
	return true;
}
bool SqlDataProvider::pushSendingBarcodesToSent(Modes mode, Entity prototype)
// This method is pushing barcodes marked as sending (2) to Uploaded db
{
	QString scanedTableName = ModePrefixes[int(mode)] + prototype->getAssociatedTable()->declaration()
		+ TableSuffixes[int(TableNames::Scanned)];
	QString uploadedTableName = ModePrefixes[int(mode)] + prototype->getAssociatedTable()->declaration()
		+ TableSuffixes[int(TableNames::Uploaded)];
	_assertAndOpenSession();
	mainDb.transaction();

	if (!execInSession(prototype->getAssociatedTable()->insert(
		prototype->getAssociatedTable()->select_filtered(" uploaded = 2", scanedTableName),
		uploadedTableName)))
	{
		mainDb.commit();
		_assertAndCloseSession();
		return false;
	}
	if (!execInSession(prototype->getAssociatedTable()
		->delete_filtered(" uploaded = 2 ", scanedTableName)))
	{
		mainDb.commit();
		_assertAndCloseSession();
		return false;
	}
	else
	{
		mainDb.commit();
		_assertAndCloseSession();
		return true;
	}
}
bool SqlDataProvider::clearSendingSelector(Modes mode, Entity prototype)
{
	QString scanedTableName = ModePrefixes[int(mode)] + prototype->getAssociatedTable()->declaration()
		+ TableSuffixes[int(TableNames::Scanned)];
	return DBexecute(
		prototype->getAssociatedTable()->update(
			" set uploaded = 1 where uploaded = 2", scanedTableName));
}
bool SqlDataProvider::pushIntoDownloaded(ShortBarcodeEntity& shb)
{
	return DBexecute(shb.getAssociatedTable()->insert(shb.asSqlInsertion()));
}
bool SqlDataProvider::pushIntoDownloaded(QLinkedList<QSharedPointer<ShortBarcodeEntity>>& l)
{
	_assertAndOpenSession();
	mainDb.transaction();
	QLinkedList<QSharedPointer<ShortBarcodeEntity>>::iterator begin = l.begin();
	while (begin != l.end())
	{
		execInSession((*begin)->getAssociatedTable()->insert((*begin)->asSqlInsertion()));
		++begin;
	}
	mainDb.commit();
	_assertAndCloseSession();
	return true;
}
Entity SqlDataProvider::barcodeInfo(QString code)
//This method is searching for barcode in downLoadedDB
{
	_assertAndOpenSession();
	QueryPtr q = runQuery(ShortBarcodeEntity::getTableHandler()->select_filtered("barcode = '" + code + "'"));
	if (q == Q_NULLPTR)
		return Entity();
	Entity proto(new ShortBarcodeEntity);
	if (!proto->fromSql(&(*(q))))
		return Entity();
	q->finish();
	_assertAndCloseSession();
	return proto;
}

EntityList SqlDataProvider::extractEntitiesForMode(Modes mode, TableNames tab, Entity prototype)
{
	EntityList Entities;
	_assertAndOpenSession();
	QString formatedName = formatTableName(mode, prototype, tab);
	QueryPtr q = runQuery(
			prototype->getAssociatedTable()->select_all(formatedName)
		);
	if (q == Q_NULLPTR)
		return Entities;
	while (prototype->fromSql(&(*q)))
	{
			Entities.push_back(Entity(prototype->clone()));
	}
	_assertAndCloseSession();
	return Entities;
}
EntityList SqlDataProvider::extractEntitiesForMode(Modes mode, TableNames tab)
{
	return extractEntitiesForMode(mode, tab, modenamesLinker[mode]);
}
QString SqlDataProvider::uploadFullList(Modes mode, Entity prototype, sendingMode sendmode, QString lang, int format, int dest)
//This method is selecting from dbname all barcodes that are ready to upload
{
	_assertAndOpenSession();
	QString scanedTableName = ModePrefixes[int(mode)] + prototype->getAssociatedTable()->declaration()
		+ TableSuffixes[int(TableNames::Scanned)];
    QueryPtr sql;
	switch (sendmode)
	{
	case sendSent:
		sql = runQuery(
			prototype->getAssociatedTable()->select_some_fields(AppSettings->serializationOrder
				[prototype->myType()],
				scanedTableName)
		);
		break;
	case sendUnsent:
		sql = runQuery
		(
			prototype->getAssociatedTable()->select_some_fields_filtered(
                " uploaded != 2"
				" order by scanedtime desc", AppSettings->serializationOrder
				[prototype->myType()], scanedTableName)
		);
		break;
	case sendAll:
		sql = runQuery(
			prototype->getAssociatedTable()->select_some_fields(AppSettings->serializationOrder
				[prototype->myType()],
				scanedTableName) +
			" union all " + prototype->getAssociatedTable()->select_some_fields(
				AppSettings->serializationOrder[prototype->myType()],
				ModePrefixes[int(mode)] + prototype->getAssociatedTable()->declaration()
				+ TableSuffixes[int(TableNames::Uploaded)]
			)
		);
	}
	if (sql == Q_NULLPTR)
		return "";
    QString formatedResponce(DataFormater::getFormatedSql(*sql, dbnameToMode(mode), lang, format, dest, int(mode)) );
	switch (sendmode)
	{
	case sendUnsent:
		sql = runQuery
		(
			prototype->getAssociatedTable()->select_filtered(
                " uploaded != 2"
				" order by scanedtime desc", scanedTableName)
		);
		break;
	case sendAll:
		sql = runQuery(
			prototype->getAssociatedTable()->select_all(
				scanedTableName) +
			" union all " + prototype->getAssociatedTable()->select_all(
				ModePrefixes[int(mode)] + prototype->getAssociatedTable()->declaration()
				+ TableSuffixes[int(TableNames::Uploaded)]
			)
		);
		break;
	default:
		break;
	}
	if (sql == Q_NULLPTR)
	{
		return QString();
	}
	mainDb.transaction();
	switch (sendmode)
	{
	case sendUnsent:
	case sendAll:
	{
		while (sql->next())
		{
			execInSession(
				prototype->getAssociatedTable()->update(" set uploaded = 2 where id = " 
					+ sql->value(0).toString() , scanedTableName
				));
		}
	}
	Q_FALLTHROUGH();
	default:
		mainDb.commit();
		_assertAndCloseSession();
		return formatedResponce;
	}
}

QString SqlDataProvider::uploadFullList(Modes mode, sendingMode sendmode, QString lang, int format, int destination)
{
	return uploadFullList(mode, modenamesLinker[mode], sendmode, lang, format, destination);
}

bool SqlDataProvider::DBexecute( QString sql)
//	This method is excecuting sql statement on database
{
	_assertAndOpenSession();
	// Are this open\close pair necessary? Possibly there must be only one open in constructor, and only commit here
	QSqlQuery q(mainDb);
#ifdef DEBUG
	detrace_METHFRECALL("execing query: " << sql);
#endif
	q.exec(sql);
	if (q.lastError().isValid()) {
#ifdef DEBUG
		detrace_METHPERROR("DBEXECUTE: " << sql << "\n", " : " + q.lastError().text());
#endif
		q.clear();
		_assertAndCloseSession();
		return false;
	}
	q.clear();
	_assertAndCloseSession();
	return true;
}

bool SqlDataProvider::execInSession(QString sql)
{
	QSqlQuery q(mainDb);
#ifdef DEBUG
	detrace_METHFRECALL("execing query: " << sql);
#endif
	q.exec(sql);
	if (q.lastError().isValid()) {
#ifdef DEBUG
		detrace_METHPERROR("DBEXECUTE: " << sql << "\n", " : " + q.lastError().text());
#endif
		q.clear();
		return false;
	}
	q.clear();
	return true;
}

bool SqlDataProvider::downloadFullList(QString upload)
//	This method is inserting into downloaded data string which is containing full received string(unparsed)
{
	recreateDownloadTable();
	return DBexecute(ShortBarcodeEntity::makeInsertionFromString(upload));
}

bool SqlDataProvider::recreateDownloadTable()
// This method drops download table
{
	DBexecute(ShortBarcodeEntity::getTableHandler()->drop());
	return DBexecute(ShortBarcodeEntity::getTableHandler()->definition());
}

bool SqlDataProvider::dropTableOf(TableNames tab, Entity prototype, Modes mode)
// drops scanned on particular database
{
	_assertAndOpenSession();
	QString fname = formatTableName(mode, prototype, tab);
	if (!mainDb.tables().contains(fname))
	{
		_assertAndCloseSession();
		return false;
	}
	execInSession(prototype->getAssociatedTable()->drop(fname));
	mainDb.commit();
	_assertAndCloseSession();
	return true;
}

bool SqlDataProvider::dropTableOf(TableNames tab, Modes mode)
{
	return dropTableOf(tab, modenamesLinker[mode], mode);
}

int SqlDataProvider::countAllIn(Modes mode, Entity prototype, TableNames tab)
{
	_assertAndOpenSession();
	mainDb.commit();
	mainDb.transaction();
	QString fname = formatTableName(mode, prototype, tab);
	QueryPtr q = runQuery(prototype->getAssociatedTable()->count(fname));
	int quantity = 0;
    if (q == Q_NULLPTR)
        return 0;
	if (q->next())
	{
		bool ok = false;
		quantity = q->value(0).toInt(&ok);
		if (!ok)
			quantity = 0;
	}
	mainDb.commit();
	_assertAndCloseSession();
	return quantity;

}

int SqlDataProvider::countAllIn(Modes mode, TableNames tab)
{
	return countAllIn(mode, modenamesLinker[mode], tab);
}

int SqlDataProvider::sumAllIn(Modes mode, unsigned int field, Entity prototype, TableNames tab)
{
	_assertAndOpenSession();
	QString fname = formatTableName(mode, prototype, tab);
    QueryPtr q = runQuery(prototype->getAssociatedTable()->sum(field, fname));
	int quantity = 0;
	if (q->next())
	{
		bool ok = false;
		quantity = q->value(0).toInt(&ok);
		if (!ok)
			quantity = 0;
	}
	_assertAndCloseSession();
	return quantity;
}

int SqlDataProvider::sumAllFilteredIn(Modes mode, const QString value, unsigned int field, Entity prototype, TableNames tab)
{
	_assertAndOpenSession();
	QString fname = formatTableName(mode, prototype, tab);
    QueryPtr q = runQuery(prototype->getAssociatedTable()->sumFieldFiltered(field, value, fname));
    if (q == Q_NULLPTR)
        return 0;
	int quantity = 0;
	if (q->next())
	{
		bool ok = false;
		quantity = q->value(0).toInt(&ok);
		if (!ok)
			quantity = 0;
	}
	_assertAndCloseSession();
	return quantity;
}

int SqlDataProvider::countUniqueIn(Modes mode, Entity prototype, TableNames tab)
{
	_assertAndOpenSession();
	QString fname = formatTableName(mode, prototype, tab);
	QSqlQuery q(mainDb);
	q.prepare(prototype->getAssociatedTable()->countUnique(QStringLiteral("barcode"), fname));
	q.exec();
	int quantity = 0;
	if (q.next())
	{
		bool ok = false;
		quantity = q.value(0).toInt(&ok);
		if (!ok)
			quantity = 0;
	}
	_assertAndCloseSession();
	return quantity;
}

int SqlDataProvider::countUniqueIn(Modes mode, TableNames tab)
{
	return countUniqueIn(mode, modenamesLinker[mode], tab);
}

bool SqlDataProvider::postEntityInto(TableNames tab, Entity entity, Modes mode)
// inserts barcode using BarcodeUniform representation
{
	QString fname = formatTableName(mode, entity, tab);
	if (entity->isValid()) 
	{
		return DBexecute
		(
			entity->getAssociatedTable()->insert(entity->asSqlInsertion(), fname)
		);
	}
	else
		return false;
}

bool SqlDataProvider::removeEntityFrom(TableNames tab, Entity entity, Modes mode)
{
	QString fname = formatTableName(mode, entity, tab);
	return DBexecute(entity->getAssociatedTable()->delete_filtered(" where barcode = '" + entity->getName() + "' "));
}

bool SqlDataProvider::doesTableExists(Modes mode, Entity prototype, TableNames tab)
{
	_assertAndOpenSession();
	QString formatedName = formatTableName(mode, prototype, tab);
	bool temp = mainDb.tables().contains(formatedName);
	_assertAndCloseSession();
	return temp;
}

bool SqlDataProvider::doesTableExists(Modes mode, TableNames tab)
{
	return doesTableExists(mode, modenamesLinker[mode], tab);
}

bool SqlDataProvider::backupEntity(Entity bc, Modes mode)
{
	if (entityExistsIn(bc, mode, TableNames::TempScan))
	{
		updateEntityIn(bc, mode, TableNames::TempScan);
	}
	else
	{
		postEntityInto(TableNames::TempScan, bc, mode);
	}
	return true;
}

bool SqlDataProvider::entityExistsIn(Entity entity, Modes mode, TableNames tab)
{
	_assertAndOpenSession();
	QSqlQuery q(mainDb);
	QString fname = formatTableName(mode, entity, tab);
	q.prepare(entity->getAssociatedTable()->select_filtered(" barcode = '" + entity->getName() + "'"));
	q.exec();
	bool res = q.next();
	q.finish();
	_assertAndCloseSession();
	return res;
}

bool SqlDataProvider::updateEntityIn(Entity entity, Modes mode, TableNames tab)
{
	removeEntityFrom(tab, entity, mode);
	postEntityInto(tab, entity, mode);
	return true;
}
SqlDataProvider* SqlDataProvider::_instanse = Q_NULLPTR;
QSqlDatabase SqlDataProvider::cloneConnection()
{
    return QSqlDatabase::cloneDatabase(mainDb, "connect" + QString::number(QTime::currentTime().msec()));
}
SqlDataProvider* SqlDataProvider::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new SqlDataProvider();
	}
	return _instanse;
}

SqlDataProvider::~SqlDataProvider()
{
	QSqlDatabase::removeDatabase("maindb.db");
}


