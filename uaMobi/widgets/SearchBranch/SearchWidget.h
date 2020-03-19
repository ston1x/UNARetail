#pragma once
#include "widgets/parents/AbstractScaningWidget.h"


class SearchWidget : public AbstractScaningWidget
{
	Q_OBJECT
protected:
	virtual void handleScanButton() override;		//	handles scan press
	virtual void handleValueFromKeyboard(QString value) override;
	virtual void barcodeReady() override;	
	virtual void _emplaceBarcode(QString barcode) override;
#ifdef CAMERA_SUPPORT
	virtual void handleCameraBarcode(QString value) override;
#endif
	virtual void _clearControls() override;
public:
	SearchWidget(QWidget* parent);
};
