#pragma once
#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include "widgets/utils/EventsAndFilters.h"
/*
		This widget is parent for everything what is going to use one-frame interface.
		Usage:
				Abstract method back() is allowing to understand, is something on top-level
			of widget tree. Overload it to allow your widget to decide when and what will be
			hidden on its branch. return true if your widget closed something on top of his branch
			and return false if this widget IS the top of his branch.
				Abstract method settings is allowing to invoke most suitable settings window. TRUE is
			returned if in this widget's branch IS custom settings screen that correspondes current top
			widget. False is returned if no settings widget was found.
				Abstract method handleBarcode is used when barcode is obtained outside of the specialized
			widgets and it must be handled immediately. Remember that this method does not guaranties
			that there will be any widget in current branch which is suited for barcode.
				Abstract method isExpectingControls is used when ambiguous number is caught in keyPress
			handlers. It returns TRUE if current branch succesfully handled this number
			and FALSE if current branch does not needs any number values and current widget can use
			them. To enhance it's functionality, it is now also carrying number - so it can be
			processed immediately.  Normal behaviour: widget calls current top's widget expecting
			controls and if FALSE is returned - this widget can use remaining value for itself.

		Update:
			overloaded show() is now virtual -> this allows to alter behavior by overloading it

		Update:
			signal BackRequired is now included by default in all inheritors, because most of them need it

		Major update:
			now this widget also provides to inheritors a keyRelease event handler
			which implements default for all inheritors behaviour - back or escape must
			call back or emit a backRequired, numbers must be passed into expectingControls.
			It is virtual, so it's okey to override it.
		Major update:
			now this widget carries its own keyfilter which is reacting on keypresses. Reactions can be overrided:
			check Reaction functions

*/

class inframedWidget : public QWidget
{
	Q_OBJECT
protected:
	filters::GeneralPurposeFilter* keyfilter;
	bool lock;
public:
	inframedWidget(QWidget* parent);
	virtual bool back();
	virtual bool giveSettings();
	virtual bool isExpectingControl(int);	//	Enhanced: now false is returned if value was not used
	filters::GeneralPurposeFilter* getFilter() { return keyfilter; };
	virtual void show();
	void installEventFilter(QObject*);
protected slots:
	virtual void returnReaction();
	virtual void backReaction();
	virtual void controlReaction(int);
	virtual void allocateLock();
	virtual void deallocateLock();
signals:
	void backRequired();
};
