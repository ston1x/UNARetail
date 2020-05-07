#pragma once
#include <QtCore/QString>
#include <QtCore/QStringList>
#ifndef QT_VERSION5X
#define noexcept
#endif
/*
	This file is holding templated table handler. main goal of this class
	is to provide common interface for tables allowing polymorthic access to them. these handlers
	are making queries filled with corresponding data. You must create an object for each table you 
	want to represent or to create one prototype and provide table name in each query.

*/



class TemplatedTableHandler
	// encapsulates table, automatizing query creation
{
protected:
	QString table_definition;	//	table schema (fields and their types)
	QString table_declaration;	//	table name (can be overrided without recreating handler)
	QStringList table_fields;	//	field names
	int primaryKeyField;		//	which field is used as primary key
	int table_type;				//	renundant type, can be used again in case of additional templates

	// checks name, returns valid one
	const QString& assertAnotherName(const QString& another_name) const noexcept;

public:
	TemplatedTableHandler();
	TemplatedTableHandler(QString t_def, QString t_decl, QStringList tfields, int primarykey = -1);
	QString declaration() const noexcept;		//	returns declaration of the table which is used to create it in database
	// returns table creation query
	QString definition(const QString another_name = QString::null) const noexcept;
	// selects all using field order provided in constructor
	QString select_all(const QString another_name = QString::null) const noexcept;
	// select using filter. Keeps column order.
	QString select_filtered(const QString& filter, const QString another_name = QString::null) const noexcept;
	// select using primary key. keeps column order
	QString select_by_primary_key(const QString pkeyvalue, const QString another_name = QString::null) const noexcept;
	// updates table
	QString select_some_fields(const QList<int>& indexes, const QString another_name = QString::null) const noexcept;

	QString select_some_fields_filtered(const QString& filter, const QList<int>& indexes, const QString another_name = QString::null) const noexcept;

	QString update(const QString& values, const QString another_name = QString::null) const noexcept;
	// replaces values using REPLACE. It guaranties that after query there will be only one value
	QString replace(const QString& values, const QString& another_name = QString::null) const noexcept;
	// deletes all non-filtered
	QString delete_filtered(const QString& filter, const QString another_name = QString::null) const noexcept;
	// deletes one element by it's primary key
	QString delete_by_primary_key(const QString& pkeyvalue, const QString another_name = QString::null) const noexcept;
	// drops table. Is a wrapper to query template
	QString drop(const QString another_name = QString::null) const noexcept;
	// returns joined names
	QString allFieldsDeclaration() const noexcept;
	// makes insertion query from provided values
	QString insert(const QString& values, const QString another_name = QString::null) const noexcept;
	// creates index for this table
	QString makeIndex(const QString& another_name = QString::null) const noexcept;
	// creates copy of this handler with another name
	TemplatedTableHandler clone(QString newName);
	// returns type of this handler. Renundant
	QStringList getFields() const noexcept;

	QString count(const QString & newName = QString::null) const noexcept;

	QString sum(const QString field, const QString& newName = QString::null) const noexcept;
	QString sum(unsigned int index, const QString& newName = QString::null) const noexcept;
	QString sumFieldFiltered(unsigned int index, const QString& value, const QString& newName = QString::null) const noexcept;
	QString countUnique(QString field, const QString & newName = QString::null) const noexcept;
};
