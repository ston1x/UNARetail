#include <QApplication>
#include <QZXing.h>
#include <QZXingFilter.h>
#include "widgets/CoreWidget.h"
#include "debugtrace.h"
//#define Q_OS_ANDROID
//	this define is used to exclude useless now localization flow

/*
	This file is constructing CoreWidget and starting all its branch

	Update: removed renundant includes from old app

*/
struct obu
{
	QString asString;
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	int scanningSessions;				//	int value - quantity of scans
	QDateTime expDate;		//	datetime of expiration. Is needed only for sending - no modifications after user input
	QString comment;
	int quantity;
};

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	debugtrace::init(detr_supply::all, detr_supply::file);
	QZXing::registerQMLTypes();
	qmlRegisterType<QZXingFilter>("QZXing", 2, 3, "QZXingFilter");
	CoreWidget cw;
	cw.show();
	AppData->downloadFullList("'1488322',32323, 'HH', '3232' ");
	return a.exec();
}