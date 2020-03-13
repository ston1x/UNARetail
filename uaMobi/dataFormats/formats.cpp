#include "formats.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QtXml/QDomDocument>
#include <QtSql/QSqlRecord>
#include <QJsonDocument>
#include <QtCore/QVariant>



QDomElement _XML_sqlColumnNames(QSqlRecord record, QDomDocument & xml)
{
	QDomElement columns{ xml.createElement("columns") };
	for (int i = 0; i < record.count(); i++)
	{
		QDomElement column{ xml.createElement("C" + QString::number(i)) };
		column.appendChild(xml.createTextNode(record.fieldName(i)));
		columns.appendChild(column);
	}
	return columns;
}

QDomElement _XML_sqlRowValues(QSqlRecord record, QDomDocument & xml)
{
	QDomElement row{ xml.createElement("row") };
	for (int i = 0; i < record.count(); i++)
	{
		QDomElement value{ xml.createElement("C" + QString::number(i)) };
		value.appendChild(xml.createTextNode(record.value(i).toString()));
		row.appendChild(value);
	}
	return row;
}

QDomElement _XML_sqlAllRowsValues(QSqlQuery query, QDomDocument & xml)
{
	QDomElement rows{ xml.createElement("rows") };
	while (query.next())
	{
		rows.appendChild(_XML_sqlRowValues(query.record(), xml));
	}
	return rows;
}

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

QString _CSV_sqlColumnNames(QSqlRecord record)
{
	QString columnsString{};
	for (int i = 0; i < record.count(); i++)
	{
		if (!columnsString.isEmpty())
		{
			columnsString += ",";
		}
		columnsString += record.fieldName(i);
	}
	return columnsString;
}

QString _CSV_sqlRowValues(QSqlRecord record)
{
	//to do need encapsulate the string variables so that we cand send them with any symbols( neded for comments )
	// record.value( i ).type() return the data type
	QString rowValuesString{};
	for (int i = 0; i < record.count(); i++)
	{
		if (!rowValuesString.isEmpty())
		{
			rowValuesString += ",";
		}
		rowValuesString += record.value(i).toString();
	}
	return rowValuesString;
}

QString _CSV_sqlAllRowsValues(QSqlQuery query)
{
	QString rowsValuesString{};
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

QString getFormatedSqlAsXml(QSqlQuery query, QString dbname)
{
	if (query.exec())
	{
		QDomDocument xml{ "file" };
		QDomElement file{ xml.createElement(dbname) };
		file.appendChild(_XML_sqlColumnNames(query.record(), xml));
		file.appendChild(_XML_sqlAllRowsValues(query, xml));
		xml.appendChild(file);
		return xml.toString();
	}
	return "";
}

QString getFormatedSqlAsJson(QSqlQuery query, QString dbname)
{
	if (query.exec())
	{
		QJsonObject json;

		json.insert("mode", dbname);

		json.insert("columns", _JSON_sqlColumnNames(query.record()));

		json.insert("rows", _JSON_sqlAllRowsValues(query));
		query.finish();

		QJsonDocument doc(json);
		return doc.toJson();
	}
	return "";
}

QString getFormatedSqlAsCsv(QSqlQuery query, QString dbname)
{
	if (query.exec())
	{
		QString csv(dbname);
		csv += "\n" + _CSV_sqlColumnNames(query.record());
		return csv + "\n" + _CSV_sqlAllRowsValues(query);
	}
	return "";
}
