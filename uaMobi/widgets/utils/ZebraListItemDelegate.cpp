#include "ZebraListItemDelegate.h"
#include "Datacore/DataEntities.h"
ZebraItemDelegate::ZebraItemDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}
static QString separator(QStringLiteral("|"));

void ZebraItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;
	QRect rct = option.rect;
	
	Entity ent = index.data(DataEntityListModel::DirectAccessRole).value<Entity>();
	if (ent == Q_NULLPTR)
		return;
	if (index.row() % 2 == 1)
	{
		painter->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
	}
	else
	{
		painter->setBrush(option.palette.background());
	}
	painter->drawRect(rct);
	painter->drawText(rct, Qt::AlignCenter | Qt::TextWordWrap, ent->maximizedView(separator, datetimeDBEncoding));
}

QSize ZebraItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Entity ent = index.data(DataEntityListModel::DirectAccessRole).value<Entity>();
	if (ent == Q_NULLPTR)
		return option.rect.size();
	return QSize(option.rect.width(),  option.fontMetrics.height() * ent->getHeight());
}
