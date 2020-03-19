#include "dataformater.h"
#include "formats.h"
#include <qsqldatabase.h>
#include <QInputDialog>

static QStringList formats{ {"xml" , "json" ,  "csv" , "txt"} };

QString inputDialogFormats(QString lang)
{
	if (lang == "Russian")
		return QInputDialog::getItem(nullptr, "Выберите формат", "Выберите формат данных", formats, -1, false);
	else if (lang == "Romanian")
		return QInputDialog::getItem(nullptr, "Selectati format", "Selectati format de data", formats, -1, false);
	else
		return QInputDialog::getItem(nullptr, "Choose format", "Select data format", formats, -1, false);
}

QString getFormat(QString lang)
{
	QString format;
	format = inputDialogFormats(lang);
	return format;
}

QString DataFormater::getFormatedSql(QSqlQuery query, QString dbname, QString lang, int format)
{
	switch (format)
	{
	case 0:
		return getFormatedSqlAsXml(query, dbname);
	case 1:
		return getFormatedSqlAsJson(query, dbname);
	case 2:
		return  getFormatedSqlAsCsv(query, dbname);
	case 3:
		return  getFormatedSqlAsTxt(query, dbname);
	default:
		QString format{ getFormat(lang) };
		if (format == "json")
		{
			return getFormatedSqlAsJson(query, dbname);
		}

		if (format == "xml")
		{
			return getFormatedSqlAsXml(query, dbname);
		}

		if (format == "csv")
		{
			return  getFormatedSqlAsCsv(query, dbname);
		}
		if (format == "txt")
		{
			return getFormatedSqlAsTxt(query, dbname);
		}
		return "";
	}
}