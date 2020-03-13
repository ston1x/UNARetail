#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/qwidget.h>
#include <QtWidgets/QBoxLayout>
#include <QObject>

#else
#include <QtGui/qwidget.h>
#include <QtGui/QBoxLayout>
#include <QObject>
#endif
#include <QString>
#include <QTime>
#include "widgets/utils/BigButtonsSpinbox.h"
#include "qsharedpointer.h"

/*
	This file provides interfaces and common features to control widgets. Main idea is to 
	incapsulate control type behind universal interfaces based on strings.
*/

namespace InputControlEntity
{
	enum ControlTypes {
		None,
		Int,
		Date,
		Label,
		Float,
		String,
		Barcode
	};
}

class abs_control : public QObject
	//control interfaces
{
	Q_OBJECT
public:

protected:
	InputControlEntity::ControlTypes type;	//	default None
	QWidget* parent;    // stored like this because inner widgets need QWidget parent, not QObject
	QString label;      // translated name


	virtual QString prepareAndReturnValue() const = 0;	//	inner function, used to return normalized value for using it into web queries
	virtual bool parseAndSetValue(QString) = 0;			//	uses normalized value to set up control
	virtual void clear() = 0;							//	drops state to default. Does not hides control
	virtual bool valueAvailable() const = 0;			//	true if value differs from default
	virtual QWidget* getInnerWidget() const = 0;        // returns key widget (with redactable field)
public:
	QString name; // not translated name, used in parsing

	explicit abs_control(QWidget* parent, 
		InputControlEntity::ControlTypes Type = InputControlEntity::None,
		QString cname = QString());
	QString getValue() const;		//	returns normalized value
	QWidget* myWidget() const;      //  wraps getInnerWidget
	InputControlEntity::ControlTypes myType();				//	return type of this control
	bool setValue(QString);				//	sets value from provided string
	void reset();						//	sets state to default
	virtual ~abs_control() = 0;         // can be usefull
	bool canGiveValue() const;
	void connectAsNext(abs_control*);
public slots:
	void subcontrolEditingFinished();   // connect here signal of inner widget to emit editingFinished
	virtual void setFocus() const  = 0;	// must set focus at redactable field with selecting all
	virtual void show() = 0;			//	shows wrapped widget
	virtual void hide() = 0;			//	hides wrapped widget
signals:
	void editingFinished();             // must be emitted on lost focus or return
	void valueReceived(QString);       // must be emitted when user confirmed value
};
typedef QSharedPointer<abs_control> Control;

