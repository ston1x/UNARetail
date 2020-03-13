#include "abstractNodeInterface.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

bool abstractNode::_hideCurrent(inframedWidget* replacement)
{
	if (current != untouchable)
	{
		current->hide();
		current = replacement;
		if (replacement == untouchable)
		{
			main->setFocus();
		}
		else
		{
			replacement->setFocus();
		}
		replacement->show();
		return true;
	}
	return false;
}

void abstractNode::_hideAny(inframedWidget* replacement)
{
	current->hide();
	current = replacement;
	replacement->setFocus();

	if (replacement == untouchable)
	{
		main->setFocus();
	}
	replacement->show();
}


abstractDynamicNode::abstractDynamicNode(inframedWidget* untouch, QLayout* mLayout)
	: mainLayout(mLayout), currentlyOpened(untouch), untouchable(untouch)
{
	if (untouch && mLayout)
	{
		mLayout->addWidget(untouch);
    }
    if (mLayout)
    {
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->setSpacing(0);
	}
}

void abstractDynamicNode::_hideAndDeleteCurrent(inframedWidget* replacement)
{
	// hides and deletes current, but stores root one
	if (replacement == Q_NULLPTR)
		return;
	if (currentlyOpened != untouchable)
	{
		mainLayout->removeWidget(currentlyOpened);
		currentlyOpened->hide();
		currentlyOpened->deleteLater();
		if (replacement != untouchable)
		{
			mainLayout->addWidget(replacement);
		}
		replacement->show();
		currentlyOpened = replacement;
		currentlyOpened->setFocus();
	}
	else 
	{
		untouchable->hide();
		mainLayout->addWidget(replacement);
		replacement->show();
		currentlyOpened = replacement;
		currentlyOpened->setFocus();
	}
}

void abstractDynamicNode::_hideAnyWithDelete(inframedWidget* replacement)
{
	// hides and deletes any widget
	if (replacement == Q_NULLPTR)
		return;
	if (currentlyOpened != Q_NULLPTR)
	{
		mainLayout->removeWidget(currentlyOpened);
		currentlyOpened->hide();
		currentlyOpened->deleteLater();
			mainLayout->addWidget(replacement);
		replacement->show();
		currentlyOpened = replacement;
		currentlyOpened->setFocus();
		return;
	}
	else
	{
		mainLayout->addWidget(replacement);
		replacement->show();
		currentlyOpened = replacement;
		currentlyOpened->setFocus();
	}
}

const char *CastFailedException::what() const
#ifdef QT_VERSION5X
    noexcept
#endif
{
#ifdef Q_OS_WINCE
    return "Error upcasting inframed pointer";
#else
     return msg.c_str();
#endif
}
