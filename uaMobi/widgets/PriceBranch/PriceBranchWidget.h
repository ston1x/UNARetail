#pragma once
#include "widgets/parents/BranchRootWidget.h"


/*
	This widget is the root of all price connected branch. All differences is contained into it's constructor-check it
	for complete understanding. In other cases, this is just a polymorthic branch root without any differences.

*/

class PriceBranchWidget : public BranchRootWidget
{
	Q_OBJECT
protected:
	// Inherited via BranchRootWidget
	virtual inframedWidget* _allocateScaning() override;
	virtual inframedWidget* _allocateStorage() override;
	virtual inframedWidget* _allocateSettings() override;
public:
	PriceBranchWidget(QWidget* parent);
};
