#include "dataformater.h"
#include "formats.h"
#include <qsqldatabase.h>
#include <QInputDialog>


QStringList _initFormats()
{
	QStringList temp;
    temp << "xml"
         << "csv" << "txt"
        #ifdef QT_VERSION5X
        << "json"
        #endif
           ;
	return temp;
}

static QStringList formats (_initFormats());


QString inputDialogFormats(QString lang)
{
	if (lang == "Russian")
		return QInputDialog::getItem(Q_NULLPTR, "Выберите формат", "Выберите формат данных", formats, -1, false);
	else if (lang == "Romanian")
		return QInputDialog::getItem(Q_NULLPTR, "Selectati format", "Selectati format de data", formats, -1, false);
	else
		return QInputDialog::getItem(Q_NULLPTR, "Choose format", "Select data format", formats, -1, false);
}

QString getFormat(QString lang)
{
	QString format;
	format = inputDialogFormats(lang);
	return format;
}

QString DataFormater::getFormatedSql(QSqlQuery query, QString dbname, QString lang, int format, int destinaton, int mode)
{
	Destinations dest = Destinations(destinaton);
	switch (format)
	{
	case 0:
		return getFormatedSqlAsXml(query, dbname, mode, dest);
	case 1:
        return  getFormatedSqlAsTxt(query, dbname, mode, dest);
	case 2:
		return  getFormatedSqlAsCsv(query, dbname, mode, dest);
#ifdef QT_VERSION5X
    case 3:
        return getFormatedSqlAsJson(query, dbname, mode, dest);
#endif
	default:
        QString format(getFormat(lang));
#ifdef QT_VERSION5X
		if (format == "json")
		{
			return getFormatedSqlAsJson(query, dbname, mode, dest);
		}
        else
#endif

		if (format == "xml")
		{
			return getFormatedSqlAsXml(query, dbname, mode, dest);
		}
        else
		if (format == "csv")
		{
			return  getFormatedSqlAsCsv(query, dbname, mode, dest);
		}
        else
		if (format == "txt")
		{
			return getFormatedSqlAsTxt(query, dbname, mode, dest);
		}
        else
		return "";
	}
}
