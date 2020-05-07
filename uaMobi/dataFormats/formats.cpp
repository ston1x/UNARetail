#include "formats.h"
#ifdef QT_VERSION5X
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#endif
#include <QtXml/QDomDocument>
#include <QtSql/QSqlRecord>
#include <QtCore/QVariant>
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/UtilityElements/ExtendedDialogs.h"
QDomElement _XML_sqlColumnNames(QSqlRecord record, QDomDocument & xml)
{
    QDomElement columns( xml.createElement("columns") );
	for (int i = 0; i < record.count(); i++)
	{
        QDomElement column(xml.createElement("C" + QString::number(i)) );
		column.appendChild(xml.createTextNode(record.fieldName(i)));
		columns.appendChild(column);
	}
	return columns;
}

QDomElement _XML_sqlRowValues(QSqlRecord record, QDomDocument & xml)
{
    QDomElement row( xml.createElement("row") );
	for (int i = 0; i < record.count(); i++)
	{
        QDomElement value(xml.createElement("C" + QString::number(i)) );
		value.appendChild(xml.createTextNode(record.value(i).toString()));
		row.appendChild(value);
	}
	return row;
}

QDomElement _XML_sqlAllRowsValues(QSqlQuery query, QDomDocument & xml)
{
    QDomElement rows( xml.createElement("rows") );
	while (query.next())
	{
		rows.appendChild(_XML_sqlRowValues(query.record(), xml));
	}
	return rows;
}
#ifdef QT_VERSION5X
QJsonArray _JSON_sqlColumnNames(QSqlRecord record)
{
	QJsonArray array;
	for (int i = 0; i < record.count(); i++)
	{
		array.push_back(record.fieldName(i));
	}
	return array;
}

QJsonArray _JSON_sqlRowValues(QSqlRecord record)
{
	QJsonArray array;
	for (int i = 0; i < record.count(); i++)
	{
		array.push_back(record.value(i).toString());
	}
	return array;
}

QJsonObject _JSON_sqlAllRowsValues(QSqlQuery query)
{
	QJsonObject rows;
	while (query.next())
	{
		rows.insert(QString::number(query.at()), _JSON_sqlRowValues(query.record()));
	}
	return rows;
}
#endif
QString _CSV_sqlColumnNames(QSqlRecord record)
{
    QString columnsString;
	for (int i = 0; i < record.count(); i++)
	{
		if (!columnsString.isEmpty())
		{
			columnsString += ";";
		}
		columnsString += record.fieldName(i);
	}
	return columnsString;
}

QString _CSV_sqlRowValues(QSqlRecord record)
{
	//to do need encapsulate the string variables so that we cand send them with any symbols( neded for comments )
	// record.value( i ).type() return the data type
    QString rowValuesString;
	for (int i = 0; i < record.count(); i++)
	{
		if (!rowValuesString.isEmpty())
		{
			rowValuesString += ";";
		}
		rowValuesString += "\"" + record.value(i).toString() + "\"";
	}
	return rowValuesString;
}

QString _TXT_fromSqlRecord(QSqlRecord rec)
{
	QString rowValuesString;
	for (int i = 0; i < rec.count(); i++)
	{
		if (!rowValuesString.isEmpty())
		{
			rowValuesString += "\t";
		}
		rowValuesString += rec.value(i).toString();
	}
	return rowValuesString + "\n";
}

QString _CSV_sqlAllRowsValues(QSqlQuery query)
{
    QString rowsValuesString;
	while (query.next())
	{
		if (!rowsValuesString.isEmpty())
		{
			rowsValuesString += "\n";
		}
		rowsValuesString += _CSV_sqlRowValues(query.record());
	}
	query.finish();
	return rowsValuesString;
}

QString getFormatedSqlAsXml(QSqlQuery query, QString dbname, int mode, Destinations destination)
{
    QDomDocument xml("file" );
    QDomElement file( xml.createElement(dbname) );
	if (destination == Destinations::NetworkDestination)
	{
		QDomElement place(xml.createElement("place"));
		place.appendChild(xml.createTextNode(AppSettings->placeAsCode));
		file.appendChild(place);
		if (AppSettings->sysfeed.at(mode) > 0)
		{
			QDomElement sysfeed(xml.createElement("sysfid"));
			sysfeed.appendChild(xml.createTextNode(QString::number(AppSettings->sysfeed.at(mode))));
			file.appendChild(sysfeed);
		}
		if (AppSettings->sendLogin)
		{

			QDomElement login(xml.createElement("login"));
			login.appendChild(xml.createTextNode(AppSettings->userLogin));
			file.appendChild(login);
			QDomElement pass(xml.createElement("password"));
			pass.appendChild(xml.createTextNode(AppSettings->userPass));
			file.appendChild(pass);
		}
	}
	file.appendChild(_XML_sqlColumnNames(query.record(), xml));
	file.appendChild(_XML_sqlAllRowsValues(query, xml));
	xml.appendChild(file);
	return xml.toString();
}
#ifdef QT_VERSION5X
QString getFormatedSqlAsJson(QSqlQuery query, QString dbname, int mode, Destinations destination)
{
		QJsonObject json;

		json.insert("mode", dbname);

		json.insert("columns", _JSON_sqlColumnNames(query.record()));

		json.insert("rows", _JSON_sqlAllRowsValues(query));
		query.finish();

		QJsonDocument doc(json);
		return doc.toJson();
}
#endif
QString getFormatedSqlAsCsv(QSqlQuery query, QString dbname, int mode, Destinations destination)
{
		QString csv;
		csv += _CSV_sqlColumnNames(query.record());
		return csv + "\n" + _CSV_sqlAllRowsValues(query);
	return "";
}

QString getFormatedSqlAsTxt(QSqlQuery query, QString dbname, int mode, Destinations destination)
{
	QString txt;
		while (query.next())
		{
			txt += _TXT_fromSqlRecord(query.record());
		}
	return txt;
}
