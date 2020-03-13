#pragma once
#include <QtWidgets/qlabel.h>
#include <QtWidgets/QLineEdit>
#include "widgets/parents/inframedWidget.h"
#include "Datacore/DataEntities.h"
#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/BigButtonsSpinbox.h"
#include <QtWidgets/QCalendarWidget>

/*
	This barcode redacting widget is concentrated over PricedBarcode datatype - it holds corresponding number of spinboxes.
	It has the same interfaces with previous barcodeUniform/ 


*/

class PricedBarcodeRedacting : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QHBoxLayout* buttonLayout;
	QLabel* aboutGeneralPrice;
	QLabel* aboutExpDates;
	QLabel* aboutDiscountPrice; // maybe must be more accented. larger font etc
	QLabel* aboutSecondaryPrice;
	QLabel* aboutUnnecessaryPrice;
	QLabel* aboutComment;
	BigButtonsSpinbox* generalPrice;
	BigButtonsSpinbox* discountPrice;
	BigButtonsSpinbox* secondaryPrice;
	BigButtonsSpinbox* unnecessaryPrice;

	QLineEdit* comment;
	QDateEdit* expDateedit;
	BigButtonsSpinbox* expTimeEdit;
	MegaIconButton* okButton;
	MegaIconButton* backButton;
	MegaIconButton* removeButton;
	filters::CaptureBackFilter* keyFilter;

	Entity barcodeCopy;

public:
	PricedBarcodeRedacting(QWidget* parent);

	void setBarcode(const Entity);    // this method is requred to set barcode. WARNING: if you forget this,
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