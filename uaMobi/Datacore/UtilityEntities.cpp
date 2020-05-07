#include "UtilityEntities.h"
#include "widgets/utils/ElementsStyles.h"



QString SeparatorEntity::_toSql() const
{
	return QString();
}


static TemplatedTableHandler* nullTTH = new TemplatedTableHandler("sep", "id number", QStringList());
const TemplatedTableHandler* SeparatorEntity::_assocTable() const
{
	return nullTTH;
}

QString SeparatorEntity::_formatedView(QString sep, QString dform) const
{
	return QString().fill('-', AppFonts->howMuchCharacterFitsIntoScreen() - 2);
}

QString SeparatorEntity::_maximumInfoView(QString sep, QString dform) const
{
	return QString().fill('-', AppFonts->howMuchCharacterFitsIntoScreen() - 2);
}

QString SeparatorEntity::_normalizedCsvView() const
{
	return QString();
}

bool SeparatorEntity::_isValid() const
{
	return true;
}

bool SeparatorEntity::_fromSql(QSqlQuery*)
{
	return true;
}

AbsEntity* SeparatorEntity::_clone() const
{
	return new SeparatorEntity();
}

QString SeparatorEntity::_getName() const
{
	return QString().fill('-', AppFonts->howMuchCharacterFitsIntoScreen() - 2);;
}

double SeparatorEntity::_getEnumerable(int role) const
{
	return -536474706.0;
}

void SeparatorEntity::_invalidate()
{
}

static QStringList nulllist;
const QStringList& SeparatorEntity::_getFields() const
{
	return nulllist;
}

QString SeparatorEntity::_fullComparationQuery() const
{
	return QString();
}

SeparatorEntity::SeparatorEntity()
	: AbsEntity(int(barcodeUtil::barcodetypes::separatorNotBC))
{
}

SeparatorEntity::~SeparatorEntity()
{
}
