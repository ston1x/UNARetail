#pragma once
#include <QtCore/QObject>
#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/QDir>
#include "Datacore/AbsEntity.h"

#define AppBackup BackupingEngine::instanse()


enum class OpType { SCAN, EDIT, BACK, REPL, DELE, UPLD, DESC, SAVE, WIPE };
enum class ModType { INV, SUP, SIM, PRI };
class BackupingEngine
{
private:
	QDir mainDir;
	QDir currentDir;
	QFile currentFile;
	QTextStream stream;
	int stringCounter;
	QString baseFilename;

	static BackupingEngine* _instanse;

	bool verify_file();
	bool verify_directory();
	bool next_file();
	bool erase_dir();
	void make_directory();
	QString make_filename(int iteration =0);
	QString make_dirname();
	void first_launch();
	BackupingEngine();
public:
	bool pushString(QString&);
	bool pushOperation(OpType opt, ModType mt, Entity barcode);
	bool pushOperation(OpType, ModType, QString supp="", QString sec="");
	bool wipeOld();
	static BackupingEngine* instanse();
};
ModType databaseToMode(int db);