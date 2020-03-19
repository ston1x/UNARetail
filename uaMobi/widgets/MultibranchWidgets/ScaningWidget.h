#pragma once
#include "widgets/parents/AbstractScaningWidget.h"
#include <QtCore/QHash>
#include "widgets/ControlsMiniwidgets/QuantityControl.h"
/*
*/

class ScaningWidget :
	public AbstractScaningWidget
{
	Q_OBJECT
protected:
	QLabel* lengthInfo;					//	length of barcode
	QLabel* totalInfo;					//	how much quantity this barcode has already
	MegaIconButton* okButton;

	QuantityControl* quantityControl;

	Barcode pendingBarcode;

	virtual void _emplaceBarcode(QString barcode) override;
	virtual void _clearControls() override;
	virtual void handleValueFromKeyboard(QString value) override;
	virtual void barcodeReady() override;
#ifdef CAMERA_SUPPORT
	virtual void handleCameraBarcode(QString value) override;
#endif
	virtual void handleScanButton();
public:
	ScaningWidget(Modes mode, QWidget* parent);

	void setTotal(int total);
	void setLen();
protected slots:					//	Slots for inner usage. To submit barcodes etc use tree intefaces
	void okPressed();
	// Inherited via AbstractScaningWidget
	//	drops backup and sends newBarcodeListCommite
};
