#include "inframedWidget.h"
//#define DEBUG
#ifdef DEBUG
#include <debugtrace.h>
#endif

using namespace filters;
bool inframedWidget::isExpectingControl(int val)
{
#ifdef DEBUG
	detrace_METHCALL(":isExpectingControl(" << val << " wadress: " << (long long int) this);
#endif

	return false;
}
inframedWidget::inframedWidget(QWidget* parent)
	: QWidget(parent), keyfilter(new filters::GeneralPurposeFilter(filters::GeneralPurposeFilter::infPack, this))
{
	QObject::installEventFilter(keyfilter);
#ifdef QT_VERSION5X
	QObject::connect(keyfilter, &GeneralPurposeFilter::backPressed, this, &inframedWidget::backReaction);
	QObject::connect(keyfilter, &GeneralPurposeFilter::returnPressed, this, &inframedWidget::returnReaction);
	QObject::connect(keyfilter, &GeneralPurposeFilter::numberPressed, this, &inframedWidget::controlReaction);
	QObject::connect(keyfilter, &GeneralPurposeFilter::scanPressed, this, &inframedWidget::allocateLock);
	QObject::connect(keyfilter, &GeneralPurposeFilter::scanReleased, this, &inframedWidget::deallocateLock);
#else
    QObject::connect(keyfilter, SIGNAL(backPressed()), this, SLOT(backReaction()));
    QObject::connect(keyfilter, SIGNAL(returnPressed()), this, SLOT(returnReaction()));
    QObject::connect(keyfilter, SIGNAL(numberPressed(int)), this, SLOT(controlReaction(int)));
#endif

}
bool inframedWidget::back()
{
	return false;
}

bool inframedWidget::giveSettings()
{
	return false;
}

void inframedWidget::show()
{
	QWidget::show();
}

void inframedWidget::installEventFilter(QObject* obj)
{
    if (obj != keyfilter)
	{
        if (keyfilter != Q_NULLPTR){

		removeEventFilter(keyfilter);
		keyfilter->deleteLater();
        keyfilter = Q_NULLPTR;
        }
        QWidget::installEventFilter(obj);
		return;
	}
}

void inframedWidget::returnReaction()
{
}

void inframedWidget::deallocateLock()
{
	lock = false;
}

void inframedWidget::backReaction()
{
	emit backRequired();
}

void inframedWidget::controlReaction(int val)
{
	if (lock)
		return;
	isExpectingControl(val - 1);
}

void inframedWidget::allocateLock()
{
	lock = true;
}
