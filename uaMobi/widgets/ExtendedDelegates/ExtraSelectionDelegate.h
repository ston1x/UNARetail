#pragma once
#include <QItemDelegate>



class ExtraSelectionDelegate :public QItemDelegate
{
	Q_OBJECT
protected:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	ExtraSelectionDelegate(QObject* parent);


};