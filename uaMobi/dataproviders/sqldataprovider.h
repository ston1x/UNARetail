#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <exception>
#include "dataFormats/dataformater.h"
#include "Datacore/DataEntities.h"
#include "dataproviders/TableHandlers.h"
#include "dataproviders/BackupingEngine.h"
#include "dataproviders/ModesDescriptions.h"
/*
		This class is responsible for storing data in inner database. It assigns to each mode 3 tables:
		scanned. uploaded and backup, using entity typename as base, tablename as suffix and modename as prefix.
		This type of architecture allows centralized changes of schema because this class is using interfaces
		of templated table handler instead of hardcoded queries. Still, some bindings are remaining hardcodes,
		like indexes or field names.
		Singletone class
*/

// this enum is used to represent type of query used for select all data for upload


#define AppData SqlDataProvider::instanse()

class DatabaseConnectionException : std::exception	//	is thrown when data can not be processed due to connection errors
{
public:
    DatabaseConnectionException(){}
    virtual const char* what() const noexcept override { return  "connection establisment failed"; }
};


// this linker represents binding of modes to entities. It must return value when mode is provided.
extern const QHash<Modes, Entity> modenamesLinker;



// creates formatted table name as described in head comment
QString formatTableName(Modes mode, Entity prototype, TableNames tab);

// used for safe transition of created but not used queries.
typedef QSharedPointer<QSqlQuery> QueryPtr;

class SqlDataProvider	//	contains database connection and incapsulated common operations
{
private:
	QSqlDatabase mainDb;		//	main database connection
	bool isSessionOpen;			//  flag is raised only if database is open by inner asserter
	
	static SqlDataProvider* _instanse; 
	
	void _assertAndOpenSession(); // opens connection. Warning - do not open connection without calling this.
	void _assertAndCloseSession(); // closes connection
	bool DBexecute(QString sql);	//	opens session. runs query, closes session
	bool execInSession(QString sql); // runs query without session closing
	QueryPtr runQuery(QString sql);  // opens session, runs query and returns it
	SqlDataProvider();
public:	
	// use prototype version for non default interactions
	EntityList extractEntitiesForMode(Modes mode, TableNames tab, Entity prototype);//	Loads Scanned table as barcodes ready to be modified
	EntityList extractEntitiesForMode(Modes mode, TableNames tab);//	Loads Scanned table as barcodes ready to be modified

	QString uploadFullList(Modes mode, Entity prototype, sendingMode sendmode, QString lang, int format = -1, int destination= 0);	//	prepares list for uploading
	QString uploadFullList(Modes mode, sendingMode sendmode, QString lang, int format = -1, int destination =0);	//	prepares list for uploading

	bool doesTableExists(Modes mode, Entity prototype, TableNames tab);  // checks table existanse
	bool doesTableExists(Modes mode, TableNames tab);

	bool dropTableOf(TableNames tab, Entity prototype, Modes mode);	//	deletes table described 
	bool dropTableOf(TableNames tab, Modes mode);	//	deletes Scanned table

	int countAllIn(Modes mode, Entity prototype, TableNames tab); // counts all entities in table/mode
	int countAllIn(Modes mode, TableNames tab);

	int sumAllIn(Modes mode, unsigned int field, Entity prototype, TableNames tab); // summs all fields in provided table.
	// Warning - field must correspond to field number, or it will be dropped to first field(id)

	int sumAllFilteredIn(Modes mode, const QString value, unsigned int field, Entity prototype, TableNames tab);
	// sums all fields by provided index, using field


	int countUniqueIn(Modes mode, Entity prototype, TableNames tab);
	int countUniqueIn(Modes mode, TableNames tab);
	// counts uniques in table using hardcoded field "barcode"
	
	bool checkAndRefreshTable(TableNames, Entity, Modes);
	bool checkAndRefreshTable(TableNames, Modes);
	// checks if table exists by insertion and deletion

	bool createTableOf(TableNames, Entity prototype, Modes);		//	same for one db
	bool createTableOf(TableNames, Modes);		//	same for one db


	bool postEntityInto(TableNames tab, Entity entity, Modes mode);	//	post in table described by table/mode
	bool removeEntityFrom(TableNames tab, Entity barcode, Modes mode);  // removes entity from described table by primary field (id)
	bool backupEntity(Entity bc, Modes mode); // pushes entity to backup table. Deprecated
	bool entityExistsIn(Entity entity, Modes mode, TableNames tab); // checks if entity exists by hardcoded field barcode
	bool updateEntityIn(Entity entity, Modes mode, TableNames tab); // updates entity with non-stable bound by barcode and quantity
	bool pushEntityList(EntityList&, Modes mode, TableNames tab);	//	drops scanned and then fills it with new barcodes

	Entity barcodeInfo(QString code);			//	Looks for barcode
	bool pushSendingBarcodesToSent(Modes, Entity);	//	moves all marked with 2 barcodes into uploaded table
	bool clearSendingSelector(Modes, Entity);		//	sets state to 1 for all barcodes in scanned
	bool pushIntoDownloaded(ShortBarcodeEntity&);    // posts barcode into downloaded table directly
	bool pushIntoDownloaded(QLinkedList<QSharedPointer<ShortBarcodeEntity> >&);

	bool downloadFullList(QString upload);	//	uses string as a value part for sql insert query
	
	bool recreateDownloadTable();				//	drops and creates DownloadTable
	bool createDefaultTables();							//	creates table with standard setup
	bool createTablesOf(const QList<Modes> dbs, Entity prototype, const QList<TableNames>);		//	creates tables on all databases

	bool deleteEntity(Modes mode, Entity toDelete, TableNames tab);    // deletes entity by hard binding to it's guid. Use this in most cases
	bool replaceEntityIn(Modes mode, Entity oldOne, Entity newOne, TableNames tab); // replaces entity by id binding. Prefer this over updateEntity
	bool wipeAll(); // deprecated

	QSqlDatabase cloneConnection(); // deprecated
	static SqlDataProvider* instanse();
	~SqlDataProvider();
};
