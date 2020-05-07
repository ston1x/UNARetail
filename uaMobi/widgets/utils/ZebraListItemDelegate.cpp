#include "ZebraListItemDelegate.h"
#include "Datacore/DataEntities.h"

#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromUtf8("" A "" + sizeof(A) - 1)
#endif
ZebraItemDelegate::ZebraItemDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}
static QString separator(QStringLiteral("|"));

void ZebraItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;
	painter->save();
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
	if (ent->myType() == int(barcodeUtil::barcodetypes::separatorNotBC))
	{
		painter->setBrush(QColor(209, 197, 237));
	}
	painter->drawRect(rct);
	painter->drawText(rct, Qt::AlignCenter | Qt::TextWordWrap, ent->maximizedView(separator, timeDBEncoding));
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
	}
	painter->restore();
}

QSize ZebraItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Entity ent = index.data(DataEntityListModel::DirectAccessRole).value<Entity>();
	if (ent == Q_NULLPTR)
		return option.rect.size();
	return QSize(option.rect.width(), ent->getHeight() * option.fontMetrics.height());
}

void ShortZebraItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;
	painter->save();
	QRect rct = option.rect;
	Entity ent = index.data(DataEntityListModel::ReversedDARole).value<Entity>();
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
	painter->drawText(rct, Qt::AlignCenter | Qt::TextWordWrap, ent->formatedView(separator, datetimeDBEncoding).left(20));
    if (option.state.testFlag(QStyle::State_Selected))
    {
        painter->setBrush(option.palette.highlight());
        painter->setOpacity(0.4);
        painter->drawRect(option.rect);
    }
	painter->restore();
}

QSize ShortZebraItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(option.rect.width(), option.fontMetrics.height() + 2);
}

ShortZebraItemDelegate::ShortZebraItemDelegate(QObject* parent)
	: ZebraItemDelegate(parent)
{
}
