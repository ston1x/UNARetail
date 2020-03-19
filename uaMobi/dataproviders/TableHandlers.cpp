#include "TableHandlers.h"



//-----------------------------------------------------------------------------------------//


const QString& TemplatedTableHandler::assertAnotherName(const QString& another_name) const noexcept
{
	return (another_name.isNull()) ? table_declaration : another_name;
}

TemplatedTableHandler::TemplatedTableHandler()
	: table_definition(), table_declaration(), table_fields(), primaryKeyField(-1), table_type(0)
{
}

TemplatedTableHandler::TemplatedTableHandler(QString t_decl, QString t_def, QStringList tfields, int primarykey)
	: table_definition(t_def), table_declaration(t_decl), table_fields(tfields), primaryKeyField(primarykey),
	table_type(1)
{
}

QString TemplatedTableHandler::declaration() const noexcept
{
	return  table_declaration;
}

QString TemplatedTableHandler::definition(const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("create table ") + name + QStringLiteral(" ") + table_definition;
}

QString TemplatedTableHandler::select_all(const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("select ") + allFieldsDeclaration() + QStringLiteral(" from ") + name;
}

QString TemplatedTableHandler::select_filtered(const QString& filter, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return select_all(name) + QStringLiteral(" where ") + filter;
}

QString TemplatedTableHandler::select_by_primary_key(const QString pkeyvalue, const QString another_name) const noexcept
{
	if (primaryKeyField == -1)
		return QString::null;
	const QString& name = assertAnotherName(another_name);
	return select_all(name) + QStringLiteral(" where ") + table_fields.at(primaryKeyField)
		+ QStringLiteral(" = ") + pkeyvalue;
}

QString TemplatedTableHandler::select_some_fields(const QList<int>& indexes, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	if (indexes.isEmpty())
	{
		return select_all( name);
	}
	QList<int>::ConstIterator index = indexes.begin();
	QString fields;
	while (index != indexes.end())
	{
		if (*index >= 0 && *index < table_fields.count())
		{
			fields += table_fields.at(*index) + ",";
		}
		++index;
	}
	fields.chop(1);
	return QStringLiteral("select ") + fields + QStringLiteral(" from ") + name;
}

QString TemplatedTableHandler::select_some_fields_filtered(const QString& filter, const QList<int>& indexes, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	if (indexes.isEmpty())
	{
		return select_filtered(filter, name);
	}
	QList<int>::ConstIterator index = indexes.begin();
	QString fields;
	while (index != indexes.end())
	{
		if (*index >= 0 && *index < table_fields.count())
		{
			fields += table_fields.at(*index) + " ";
		}
		++index;
	}
	fields.chop(1);
	return QStringLiteral("select ") + fields + QStringLiteral(" from ") + name + QStringLiteral(" where ")
		+ filter;
}

QString TemplatedTableHandler::update(const QString& values, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("update ") + name + QStringLiteral(" ") + values;
}

QString TemplatedTableHandler::replace(const QString& values, const QString& another_name) const noexcept
{
	const QString& name = (another_name.isNull()) ? table_declaration : another_name;
	return QStringLiteral("REPLACE INTO ") + name + QStringLiteral(" ( ")
		+ allFieldsDeclaration() + QStringLiteral(" ) VALUES ") + values;
}

QString TemplatedTableHandler::delete_filtered(const QString& filter, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("delete from ") + name + QStringLiteral(" where ") + filter;
}

QString TemplatedTableHandler::delete_by_primary_key(const QString& pkeyvalue, const QString another_name) const noexcept
{
	if (primaryKeyField == -1)
		return QString::null;
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("DELETE FROM ") + name + QStringLiteral(" where ")
		+ table_fields.at(primaryKeyField) + " = " + pkeyvalue;
}

QString TemplatedTableHandler::drop(const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("drop table ") + name;
}

QString TemplatedTableHandler::allFieldsDeclaration() const noexcept
{
	return table_fields.join(" , ");
}

QString TemplatedTableHandler::insert(const QString& values, const QString another_name) const noexcept
{
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("insert into ") + name + QStringLiteral(" (")
		+ allFieldsDeclaration() + QStringLiteral(") values ") + values ;
}

QString TemplatedTableHandler::makeIndex(const QString& another_name) const noexcept
{
	if (primaryKeyField == -1)
		return QString::null;
	const QString& name = assertAnotherName(another_name);
	return QStringLiteral("CREATE INDEX ") + name + QStringLiteral("_index ON ")
		+ name + "(" + table_fields.at(primaryKeyField) + ")";
}

TemplatedTableHandler TemplatedTableHandler::clone(QString newName)
{
	return TemplatedTableHandler(newName, table_declaration, table_fields, primaryKeyField);
}

QStringList TemplatedTableHandler::getFields() const noexcept
{
	return table_fields;
}

QString TemplatedTableHandler::count(const QString& newName) const noexcept
{
	const QString& name = assertAnotherName(newName);
	return "SELECT count(*) from " + name;
}

QString TemplatedTableHandler::sum(const QString field, const QString& newName) const noexcept
{
	const QString& name = assertAnotherName(newName);
	if (!table_fields.contains(field))
		return  "SELECT sum(" + table_fields.at(0) + ") from " + name;
	return "SELECT sum(" + field + ") from " + name;
}

QString TemplatedTableHandler::sum(unsigned int index, const QString& newName) const noexcept
{
	const QString& name = assertAnotherName(newName);
	if (index >= table_fields.count())
		return "SELECT sum(" + table_fields.at(0) + ") from " + name;
	return "SELECT sum(" + table_fields.at(index) + ") from " + name;
}

QString TemplatedTableHandler::sumFieldFiltered(unsigned int index, const QString& value, const QString& newName) const noexcept
{
	const QString& name = assertAnotherName(newName);
	if (index >= table_fields.count())
		return "SELECT sum(" + table_fields.at(0) + ") from " + name + " where " + table_fields.at(0) + " = " + value;
	return "SELECT sum(" + table_fields.at(index) + ") from " + name + " where " + table_fields.at(0) + " = " + value;
}

QString TemplatedTableHandler::countUnique(const QString field, const QString& newName) const noexcept
{
	QString name = assertAnotherName(newName);
	if (field.isEmpty())
	{
		if (primaryKeyField != -1)
			return QStringLiteral("SELECT count(DISTINCT ") +
			table_fields.at(primaryKeyField) + QStringLiteral(") from ") + name;
		else
			return QStringLiteral("SELECT count(DISTINCT ") +
			table_fields.first() + QStringLiteral(") from ") + name;
	}
	else
	{
		if (table_fields.contains(field))
			return QStringLiteral("SELECT count(DISTINCT ") + field + QStringLiteral(") from ") + name;
		else
			return QString();
	}
	
}
