#pragma once
#include "widgets/parents/AbstractScaningWidget.h"

/*
	This widget is minified specialization of Scaning Widget. While it is deriving all
	control elements - barcode redacting window and scroll bar are hidden but not deleted.
	Reason: one day redacting may be usefull, also this saves writing time in cost of 2-3mb RAM.
	If you have some time, you can make abstract scan widget.

	handleBarcode - instead of inserting barcode, it launches query on Downloaded DB. Result is
	pushed to barcodelist.
	back overloaded - no need in additional branches
	Now this widget is storing GlobalAppSettings - for using sqldataprov.

	MAJOR UPDATE:
		Removed renundant inheriting: now this derives AbstractScaningWidget which is providing simple
		interface.
	update:
		Now this uses handle overrides to obtain confirmations
*/

class SearchWidget : public AbstractScaningWidget
{
	Q_OBJECT
protected:
	virtual void handleScanButton();		//	handles scan press
	virtual void handleValueFromKeyboard(QString value);
	virtual void barcodeReady();	
	virtual void _emplaceBarcode(QString barcode);
	virtual void _clearControls();
public:
	SearchWidget(QWidget* parent);
};
