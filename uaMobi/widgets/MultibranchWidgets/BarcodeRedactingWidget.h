#pragma once
#include <QtWidgets/QTextEdit>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/QLineEdit>
#include "widgets/parents/inframedWidget.h"
#include "Datacore/DataEntities.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/BigButtonsSpinbox.h"
#include <QtWidgets/QCalendarWidget>
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
	QVBoxLayout* mainLayout;
	QHBoxLayout* buttonLayout;
	QLabel* aboutForm;
	QLabel* aboutQuantity;
	QLabel* aboutExpDates;
	QLabel* aboutBarcode; // maybe must be more accented. larger font etc
	QLabel* aboutComment;
	BigButtonsSpinbox* quantity;
	QTextEdit* comment;
	QDateEdit* expDateedit;
	BigButtonsSpinbox* expTimeEdit;
	MegaIconButton* okButton;
	MegaIconButton* backButton;
	MegaIconButton* removeButton;
	filters::CaptureBackFilter* keyFilter;

	Entity entityCopy;

public:
	BarcodeRedactingWidget(QWidget* parent);

	void setBarcode(const Entity&);    // this method is requred to set barcode. WARNING: if you forget this,
	//			form will still be created, and will submit result - just with empty barcode values
	void setTimeFormat(const QString);			//	this method sets time format which will be used in displaying

protected slots:
	void okPressed();
	void backPressed();
	void removePressed();
signals:
	void barcodeCommited(Entity);			//	commit changed barcode
	void backRequired();							//	exit without save
};
