#pragma once
#include <QtCore/QObject>
#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/QDir>
#include "Datacore/AbsEntity.h"

/*
	This file contains  backuping engine which is storing in csv files main events in the application.
	It saves actions in files stored in folder sequence /year/month/day/
	It has only one method - push operation, which describes and stores provided operation.

	Singletone class.
*/
#define AppBackup BackupingEngine::instanse()


enum
#ifdef QT_VERSION5X
        class
#endif
OpType { 
	SCAN, // scaning 
	EDIT, // barcode edited
	BACK, // barcode backuped
	REPL, // barcode replaced
	DELE, // barcode deleted
	UPLD, // barcode uploaded
	DESC, // description of current action
	SAVE, // barcodes saved
	WIPE  // database was wiped
};
enum
        #ifdef QT_VERSION5X
                class
        #endif
ModType { INV, SUP, SIM, PRI, INVC }; // duplicated Modes from sqldataprovider
class BackupingEngine
	// pushes short operation description into log files
{
private:
	QDir mainDir; // root directory
	QDir currentDir; // current directory (day)
	QFile currentFile; // current file used to save data
	QTextStream stream; // current stream connected to file
	int stringCounter; // counter which is limiting total strings in file
	QString baseFilename; // base name which is used as a template baseFilename + current iteration.csv

	static BackupingEngine* _instanse;

	bool verify_file(); // checks if file is open and did not exceeded string limit.
	bool verify_directory(); // checks if directories named by date exist
	bool next_file();		//	creates new file with incremented counter
	bool erase_dir();		//	erases directory. NOT IMPLEMENTED
	void make_directory();  //  creates new directory named by date
	QString make_filename(int iteration =0); // makes filename appending iteration number to template
	QString make_dirname(); // makes directory name from date
	void first_launch(); // creates root directory UOLogs
	bool _assertOptMode(OpType opt, int mode);
	BackupingEngine();    // private to avoid duplication
public:
	bool pushString(QString&); // direct string push. Is safe for csv formatting
	bool pushOperation(OpType opt, int mt, Entity barcode); // pushes operation over provided Entity
	bool pushOperation(OpType, int, QString supp="", QString sec=""); // pushes operation description by two strings
	bool wipeOld(); // destroys all files and folders which are from other year or 3 months older
	static BackupingEngine* instanse();
};
ModType databaseToMode(int db); // utility - safely transforms int to modtype
