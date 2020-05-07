#pragma once

#include <QtCore/QPointer>
#include <QLayout>
#include "widgets/parents/inframedWidget.h"
/*
	Attempts to make uniform interface for any node

*/

class CastFailedException : public std::exception
// is thrown if cast failed during upcasting widget
{
#ifdef Q_OS_WINCE
public:
    CastFailedException(QString str) { }
    virtual const char* what() const override;

#else
private:
    std::string msg;
public:
    CastFailedException(QString str) { msg =  "Error upcasting inframed pointer with from type " + str.toStdString(); }
    virtual const char* what() const
#ifdef QT_VERSION5X
    noexcept
#endif
    override;
#endif
};
class abstractNode
{
protected:
	inframedWidget* current;
	inframedWidget* untouchable;
	inframedWidget* main;

    abstractNode() : current(Q_NULLPTR), untouchable(Q_NULLPTR), main(Q_NULLPTR) {}
    abstractNode(inframedWidget* c, inframedWidget* u, inframedWidget* m) : current(c), untouchable(u), main(m) {}
public:
    virtual ~abstractNode(){}
public:
	virtual bool _hideCurrent(inframedWidget* replacement);
	virtual void _hideAny(inframedWidget* replacement);
};


class abstractDynamicNode
	// provides interfaces for dynamic node - auto emplacing of widgets, auto deleting
{
protected:
	// layout where widgets are emplaced
	QLayout* mainLayout;
	// widget which is now shown
	inframedWidget* currentlyOpened;
	// widget which must never be deleted (root one)
	inframedWidget* untouchable;
public:
    abstractDynamicNode(inframedWidget* untouch = Q_NULLPTR, QLayout* mLayout = Q_NULLPTR);
	// hides current, but respects root widget
	virtual void _hideAndDeleteCurrent(inframedWidget* replacement);
	// hides any widget. Use this if your root is too heavy or never used later
	virtual void _hideAnyWithDelete(inframedWidget* replacement);
	template <class T>
	T* _upCO()
		// upcasts currently opened widget, thus allowing you to connect it as normal
	{
		T* temp = qobject_cast<T*>(currentlyOpened);
        if (temp == Q_NULLPTR)
		{
			throw CastFailedException(currentlyOpened->objectName());
		}
		return temp;
	}
    virtual ~abstractDynamicNode(){}
};
