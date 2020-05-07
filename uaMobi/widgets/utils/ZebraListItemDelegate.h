#pragma once
#include <QStyledItemDelegate>
#include <QtGui/QPainter>

/*
	This class is realization of simple zebra-like delegate which is coloring odd rows in
	gray color. Use it as any other delegate.

	Update:
		Now this delegate checks for separating line in value. If it is found, delegate will paint it with blue

*/

class ZebraItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
protected:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	ZebraItemDelegate(QObject* parent);
	

};

class ShortZebraItemDelegate : public ZebraItemDelegate
{
	Q_OBJECT
protected:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
public:
	ShortZebraItemDelegate(QObject* parent);
};
