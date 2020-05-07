#include "ExtraSelectionDelegate.h"
#include <QPainter>

void ExtraSelectionDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();
	QRect rct = option.rect;
	bool isSelected = index.data(Qt::UserRole + 1).toBool();
	painter->setBrush(option.palette.background());
	painter->drawRect(rct);
	painter->drawText(rct, Qt::AlignCenter | Qt::TextWordWrap, index.data(Qt::DisplayRole).toString());
	if (option.state.testFlag(QStyle::State_Selected))
	{
		painter->setBrush(option.palette.highlight());
		painter->setOpacity(0.4);
		painter->drawRect(option.rect);
	}
	if (isSelected)
	{
		painter->setBrush(QColor(217, 199, 237));
		painter->setOpacity(0.5);
		painter->drawRect(option.rect);
	}
	painter->restore();
}

QSize ExtraSelectionDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(option.rect.width(),  option.fontMetrics.height() * 2);
}

ExtraSelectionDelegate::ExtraSelectionDelegate(QObject* parent)
	: QItemDelegate(parent)
{

}
