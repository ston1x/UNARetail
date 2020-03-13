#pragma once
#include <QtCore/QString>
#include <QtCore/qurl.h>
#include "dataproviders/sqldataprovider.h"
#include <QtCore/QTranslator>


/*
		This class is responsible for global settings.
*/

extern const float VERSION;
extern const char* SUFFIX;

#define AppSettings GlobalAppSettings::instanse()

class GlobalAppSettings	//	Holds main settings for data exchange. DO NOT MAKE MULTIPLE
{
private:
	static GlobalAppSettings* _instanse;
	explicit GlobalAppSettings();		//	Explicit default constructor. All data is obtained automatically via file
public:
	QString localfile;		//	file for tolocalmemory
	QUrl httpOut;				//	address for tohttp upload
	QUrl httpIn;			//	address for tohttp download

	bool additionalControlElements;	//	controls panel with camera\ keyboard shortcuts
	bool navigationElements;

	bool autoSearch;				//	enables search on new barcode
	bool simpleSending;
	int sendingDirection;
	int sendingFormat;

	int scanPrefix;
	int scanSuffix;
	int scanButtonCode;

	QString language;					//	language of the application
	QTranslator qt_translator;			//	global translator is stored here

	void SetTranslator();				//	Sets translator. Can be used any time
	void Save();						//	Forse save
	~GlobalAppSettings();				//	Dumping destructor. Saves state before exit
	static GlobalAppSettings* instanse();
};
