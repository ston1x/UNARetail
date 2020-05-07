#pragma once
#include "widgets/parents/BranchRootWidget.h"

/*
	This widget is supposed to be root of simple branch. Now it differs only
	by database name, but sometimes it will be changed.

*/

class SimpleBranchWidget : public BranchRootWidget
{
	Q_OBJECT
protected:
	// Inherited via BranchRootWidget
	virtual inframedWidget* _allocateScaning() override;
	virtual inframedWidget* _allocateStorage() override;
	virtual inframedWidget* _allocateSettings() override;
public:
	SimpleBranchWidget(QWidget* parent);
	virtual void backRequire() override;
};
