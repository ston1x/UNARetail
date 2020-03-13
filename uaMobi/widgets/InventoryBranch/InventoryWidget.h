#pragma once
#include "widgets/parents/BranchRootWidget.h"

/*
	This is child of BranchRootWidget which will be forming unique InventoryWidget behaviour.
	Now it just changes dbname to Inventory.

	Update: really big lightening update - more common functions moved to Branch Root.
		Now this wrapper has no own functional - possibly this widget can be removed
*/

class InventoryWidget :
	public BranchRootWidget
{
	Q_OBJECT
protected:
	// Inherited via BranchRootWidget
	virtual inframedWidget* _allocateScaning() override;
	virtual inframedWidget* _allocateStorage() override;
	virtual inframedWidget* _allocateSettings() override;
public:
	InventoryWidget(QWidget* parent);
};
