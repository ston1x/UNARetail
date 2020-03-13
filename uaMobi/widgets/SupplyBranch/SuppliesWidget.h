#pragma once
#include "widgets/parents/BranchRootWidget.h"

/*
	This widget is core for Supply-branch. It has its own settings form, its own childs
	and will be splitting in 3 branches.
	innerWidget is used to make an independently-hiding element which does not require more subclasses.

	TODO : finish receive functionality
		   Add pre-appexit backup of data (NOT IN DESTRUCTOR! Bad idea)

	Update:
		Scanner branch completed - now it's fully functional
		Scanned branch completed - now it's functional and connected to scanner branch
		receive branch visual completed
		Global app settings link added
		new public slots added
	Update:
		drop scanned slot added - it drops associated table in database.
		Added connection to dataprov[Scan] database - now vector with barcodes is forming from the query and
		after exit is loaded back into db.
	Update:
		All similar with Inventory and Simple branches were moved to basic BranchRootWidget because
		their interfaces and behaviour are quite similar.
	Update:
		more similarities are moved to Branch root. This widget can be constructed without this
		child.
*/
class SuppliesWidget :
	public BranchRootWidget
{
	Q_OBJECT
protected:
	// Inherited via BranchRootWidget
	virtual inframedWidget* _allocateScaning() override;
	virtual inframedWidget* _allocateStorage() override;
	virtual inframedWidget* _allocateSettings() override;
public:
	SuppliesWidget( QWidget* parent);
};
