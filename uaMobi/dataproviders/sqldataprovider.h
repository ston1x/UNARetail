#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <exception>
#include "dataFormats/dataformater.h"
#include "Datacore/DataEntities.h"
#include "dataproviders/TableHandlers.h"
#include "dataproviders/BackupingEngine.h"
/*
		This class is responsible for storing data in any of inner databases. It must not be
		created multiple times: possibly it should be instantiated. It holds 4 database
		connections which can be accessed through enumeration DatabaseNames.
		Also DatabaseConnectionException is thrown when something is wrong with connection
		- databases are not open, errors ocurred, etc

		DatabaseNames  - are holding 4 names of databases. These are not corresponding to mainPage
		modes - possibly they are linked with "modes" of data processing - Inventory is main db with
		last-modified behaviour, Receive is used while receiving data, search is used for quick scans
		and scan is used to store new entries.

		MAJOR update:
			now database names are equal to application modes
			now literals are partially wiped from method bodies
			now datetime serialization fixed - no more empty values
			additional method appeared - execInSession is executing statement on database while
			it is already opened. Is really faster, but throws DataConnectException
			lot of detracing added
			all methods remained from ovidiu marked as ":LEGACY:" are kept only to avoid inner
			mismatched. They must be deleted later.

		MAJOR update:
			now only unsent barcodes are moved
			now barcodes are storing their sending state: 1 - ready 2 - in process
			lot of changes in sending - receiving: now common operations are moved to table handlers
			which are allowing to make them using abstract interface, i.e. now drop or select can be used
			on any table
			Now common queries are incapsulated in table handlers, leaving only small part to be defined in
			methods
			Additional methods added: they are related to Uploaded table, allowing pushing barcodes
			into it.
			All possible methods now are more abstact: if possible, table and database are provided
			in arguments

		MAJOR update:
			Now this class supports abstract barcodes interface, which main goal is to allow any barcode to carry
			it's own table bingings, so any descendant of abs_barcode could be used in any sqldataprovider's
			method without any branching. Do not forget to overwrite ALL methods in barcode descendants - it's important
			for correct work with this class.
																	--Written by Ovidiu.-- Rewritten by Anke
*/

enum sendingMode { sendSent, sendUnsent, sendAll };


#define AppData SqlDataProvider::instanse()

class DatabaseConnectionException : std::exception	//	is thrown when data can not be processed due to connection errors
{
	char msg[32] = "connection establisment failed";
public:
	using std::exception::exception;
	virtual const char* what() const noexcept override { return msg; }
};

enum class Modes
{
	Search, Inventory, Supplies, Simple, Prices
};

inline uint qHash(Modes m)
{
	return uint(m);
}

extern const QHash<Modes, Entity> modenamesLinker;

QString formatTableName(Modes mode, Entity prototype, TableNames tab);

class SqlDataProvider	//	contains database connections. DO NOT MULTIPLY
{
private:
	QSqlDatabase mainDb;		//	these are corresponding to DatabaseNames
	bool isSessionOpen;
	
	static SqlDataProvider* _instanse;
	
	void _assertAndOpenSession();
	void _assertAndCloseSession();
	bool DBexecute(QString sql);	//	This is inner method for executing on one of DB's
	bool execInSession(QString sql);
	std::shared_ptr<QSqlQuery> runQuery(QString sql);
	SqlDataProvider();
public:	
	// use prototype version for non default interactions
	EntityList extractEntitiesForMode(Modes mode, TableNames tab, Entity prototype);//	Loads Scanned table as barcodes ready to be modified
	EntityList extractEntitiesForMode(Modes mode, TableNames tab);//	Loads Scanned table as barcodes ready to be modified

	QString uploadFullList(Modes mode, Entity prototype, sendingMode sendmode, QString lang, int format = -1);	//	prepares list for uploading
	QString uploadFullList(Modes mode, sendingMode sendmode, QString lang, int format = -1);	//	prepares list for uploading

	bool doesTableExists(Modes mode, Entity prototype, TableNames tab);
	bool doesTableExists(Modes mode, TableNames tab);

	bool dropTableOf(TableNames tab, Entity prototype, Modes mode);	//	deletes Scanned table
	bool dropTableOf(TableNames tab, Modes mode);	//	deletes Scanned table

	int countAllIn(Modes mode, Entity prototype, TableNames tab);
	int countAllIn(Modes mode, TableNames tab);

	int sumAllIn(Modes mode, unsigned int field, Entity prototype, TableNames tab);

	int countUniqueIn(Modes mode, Entity prototype, TableNames tab);
	int countUniqueIn(Modes mode, TableNames tab);
	
	bool checkAndRefreshTable(TableNames, Entity, Modes);
	bool checkAndRefreshTable(TableNames, Modes);

	bool createTableOf(TableNames, Entity prototype, Modes);		//	same for one db
	bool createTableOf(TableNames, Modes);		//	same for one db


	bool postEntityInto(TableNames tab, Entity entity, Modes mode);	//	post in TABLE BARCODE on DATABASE
	bool removeEntityFrom(TableNames tab, Entity barcode, Modes mode);
	bool backupEntity(Entity bc, Modes mode);
	bool entityExistsIn(Entity entity, Modes mode, TableNames tab);
	bool updateEntityIn(Entity entity, Modes mode, TableNames tab);
	bool pushEntityList(EntityList&, Modes mode, TableNames tab);	//	drops scanned and then fills it with new barcodes

	QString barcodeInfo(QString code);			//	Looks for barcode
	bool pushSendingBarcodesToSent(Modes, Entity);	//	moves all marked with 2 barcodes into uploaded table
	bool clearSendingSelector(Modes, Entity);		//	sets state to 1 for all barcodes in scanned

	bool downloadFullList(QString upload);	//	same, but for downloading
	
	bool recreateDownloadTable();				//	drops and creates DownloadTable
	bool createDefaultTables();							//	creates table with standard setup
	bool createTablesOf(const QList<Modes> dbs, Entity prototype, const QList<TableNames>);		//	creates tables on all databases

	bool wipeAll();

	QSqlDatabase cloneConnection();
	static SqlDataProvider* instanse();
	~SqlDataProvider();
};
