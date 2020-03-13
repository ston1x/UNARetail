#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qdatetimeedit.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/QLabel>
#else
#include <QtGui/QWidget>
#include <QtGui/QSpinBox>
#include <QtGui/qpushbutton.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qdatetimeedit.h>
#include <QtGui/qgridlayout.h>
#include <QtGui/QLabel>
#endif

#include "widgets/utils/EventsAndFilters.h"

/*
	This class is a wrapper for any normal spinBox, providing it with big and suitable
	buttons for steps. For specifying which spinbox is added into it->check spintype.
	It uses RTTI for connecting to corresponding slots, so you must alter this class to add
	new spinbox type in it. Now supported: SpinBox, DatetimeEdit and all their inheritors.

	Update:
		Now Keyfilter is filtering input to get back presses. Signal backRequired is emitted
		on <- key.
	Update:
		now floatspin is supproted. It shares all interface with intspin except signals.
*/

class ReturnReactingSpinBox : public QSpinBox
{
	Q_OBJECT
protected:
	virtual void keyReleaseEvent(QKeyEvent*) override;
public:
	ReturnReactingSpinBox(QWidget* parent);
signals:
	void returnPressed();
};

class ReturnReactingDoubleSpinBox : public QDoubleSpinBox
{
	Q_OBJECT
protected:
	virtual void keyReleaseEvent(QKeyEvent*) override;
public:
	ReturnReactingDoubleSpinBox(QWidget* parent);
signals:
	void returnPressed();
};

class ReturnReactingDateEdit : public QDateEdit
{
	Q_OBJECT
protected:
	virtual void keyReleaseEvent(QKeyEvent*) override;
public:
	ReturnReactingDateEdit(QWidget* parent);
signals:
	void returnPressed();
};

class ReturnReactingDateTimeEdit : public QDateTimeEdit
{
	Q_OBJECT
protected:
	virtual void keyReleaseEvent(QKeyEvent*) override;
public:
	ReturnReactingDateTimeEdit(QWidget* parent);
signals:
	void returnPressed();
};

class ReturnReactingTimeEdit : public QTimeEdit
{
	Q_OBJECT
protected:
	virtual void keyReleaseEvent(QKeyEvent*)override;
public:
	ReturnReactingTimeEdit(QWidget* parent);
signals:
	void returnPressed();
};


class BigButtonsSpinbox : public QWidget
	//	Spinbox with two GIANT buttons which are replacing it's original
{
	Q_OBJECT
public:
	enum spintype { intspin, timespin, floatspin, datespin, datetimespin };	//	Used to determine which type of spinbox is wrapped

private:
	QGridLayout* mainLayout;			//	View
	QPushButton* buttonUp;
	QPushButton* buttonDown;
	QLabel* infoLabel;
	QAbstractSpinBox* coreSpinbox;	//	Abstract spinbox allows to place here any spinbox
	filters::CaptureBackFilter* keyFilter;		//	Captures back press

	spintype sptype;				//	Stores type for correct usage of inner functions,

	QTime lastEmit;

	virtual void showEvent(QShowEvent*) override;
public:
	BigButtonsSpinbox(spintype type, QWidget* parent, double adaptH = 0.125);
	//	methods of QSpinBox interface
	void setMinimum(int min);	// sets maximum
	void setMaximum(int max);	// sets minimum
	void setValue(int val);		//	sets current value
	void setDValue(double val);
	int value() const;				//	returns current value
	double dvalue() const;
	//	methods of QTimeEdit interface
	void setTime(const QTime& tm);	//	sets current value
	QTime time();			//	returns current value
	void setDisplayFormat(const QString& tf);//	sets format
	//methods of QDateEdit
	QDate getDate();
	void setDate(const QDate& dt);
	// methods of QDateTimeEdit
	QDateTime getDateTime();
	void setDateTime(const QDateTime& dt);
	bool hasFocus() const;
	void setInfo(QString&);
	void selectAll();
	void setStyleSheet(const QString&);
private slots:
	void intValueChanged(int);				//	These slots are wrapping slots of inner spinbox
	void timeValueChanged(const QTime& t);
	void doubleValueChanged(double);
	void editingDone();
	void backRequire();


public slots:
	void setFocus() const;			//	Sets focus to spinbox

signals:
	void ivalueChanged(int);			//	emitted only when SpinBox is wrapped
	void timeChanged(const QTime& t);	//	emitted only when TimeEdit is wrapped
    void valueChanged(QString);
	void dvalueChanged(double);
	void dateChanged(const QDate& d);
	void dateTimeChanged(const QDateTime& dtt);
	void returnPressed();				//	return was pressed
	void backRequired();				//	back was pressed
};
