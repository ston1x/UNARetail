#pragma once
#include <QSqlQuery>

/*
	This file provides only one function - getFormatedSql, which is providing to user dialog and
	then converts query to chosen format. Namespace is used only for keeping old syntax.
*/

namespace DataFormater
{
	extern QString getFormatedSql(QSqlQuery query, QString dbname, QString lang, int format = -1);
};
