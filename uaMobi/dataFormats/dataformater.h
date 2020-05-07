#pragma once
#include <QSqlQuery>
/*
	This file provides only one function - getFormatedSql, which is providing to user dialog and
	then converts query to chosen format. Namespace is used only for keeping old syntax.
	Formats:
	0 xml
	1 csv
	2 json\txt
	3 txt if json exists
*/

namespace DataFormater
{
	extern QString getFormatedSql(QSqlQuery query, QString dbname, 
		QString lang, int format = -1,
		int dest = 0, 
		int mode = 0);
};
