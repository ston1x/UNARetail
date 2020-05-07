#pragma once
#include "widgets/MultibranchWidgets/Redactors/BarcodeRedactorForm.h"
#include "widgets/MultibranchWidgets/Redactors/PricedBarcodeRedactorForm.h"
#include "widgets/parents/inframedWidget.h"
#include "dataproviders/sqldataprovider.h"
#include <QGridLayout>
/*
	This class responds for redacting barcode entry. It has signals
		barcodeCommited() - holds new barcode entry
		backRequired() - just asks for hiding

	Also here is BarcodeUniform structure - it is required to hold uniform barcode
	entry with all necessary info.

	Update:
		All buttons changed to megabuttons. Added styles -> check ElementsStyles to see them
		New layout: buttonLayout is added for buttons only. Now buttons are nested there.

	Update:
		minor interface changes
	Update:
		Now spinboxes are reworked: new subclass named BigButtonSpinbox which is adding
		a big buttons for stepping.
	Update:
		Now filter is listening back key in child widgets
		Now back key is successfully working on all the widget

*/

class BarcodeRedactingWidget : public inframedWidget
{
	Q_OBJECT
private:
	QGridLayout* mainLayout;
	
	Abs_Redactor* redactor;
	MegaIconButton* okButton;
	MegaIconButton* backButton;


	Entity entityCopy;
	Entity originalLink;
public:
	BarcodeRedactingWidget(Modes mode, QWidget* parent);

	bool setBarcode(const Entity&);    // this method is requred to set barcode. 
protected slots:
	void okPressed();
	void backPressed();
signals:
	void barcodeCommited(Entity, Entity);			//	commit changed barcode
};
