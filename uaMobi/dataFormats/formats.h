#pragma once
#include <QtSql/QSqlQuery>

/*
		This file is replacing renundant classes dedicated to formatting because
		these classes did nothing simple fuction can not do.
		All functions are receiving query and then transform it into string extracting column names
		and values. This header's definition contains some extra functions that are used only as nested.

*/


QString getFormatedSqlAsXml(QSqlQuery query, QString dbname);
QString getFormatedSqlAsJson(QSqlQuery query, QString dbname);
QString getFormatedSqlAsCsv(QSqlQuery query, QString dbname);