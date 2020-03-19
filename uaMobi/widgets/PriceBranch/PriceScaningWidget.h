#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include "widgets/ControlsMiniwidgets/QuantityControl.h"


class PriceScaningWidget :
	public AbstractScaningWidget
{
	Q_OBJECT
protected:
	QLabel* lengthCounter;
	QLabel* totalCounter;				//	how much times user scanned this barcode
	
	QuantityControl* generalPrice;
	QuantityControl* discountPrice;
	
	MegaIconButton* okButton;

	PricedBarcode pendingBarcode;

	// Inherited via AbstractScaningWidget
	virtual void _emplaceBarcode(QString barcode) override;
	virtual void _clearControls() override;
	virtual void handleValueFromKeyboard(QString value) override;
	virtual void handleScanButton() override;
	virtual void barcodeReady() override;

#ifdef CAMERA_SUPPORT
	virtual void handleCameraBarcode(QString value) override;
#endif
public:
	PriceScaningWidget(QWidget* parent);
	
	void setLen();
	void setTotal(int total);
	virtual void show() override;
protected slots:					//	Slots for inner usage. To submit barcodes etc use tree intefaces
	void okPressed();	

};
