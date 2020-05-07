#pragma once
#ifdef CAMERA_SUPPORT
#include "widgets/parents/AbstractCameraWidget.h"

/*
	This widget differs from it's parent mostly by it's qml code. This version is including
	working QZXing filter which is scanning barcodes in frame stream.

	signal hasBarcode: is emitted when camera detected a barcode

	slot barcodeObtained: is connected to qml object's signal bcArr -> which is emitted when decoding
	was created working code.

*/

class ScaningCameraWidget : public AbstractCameraWidget
{
	Q_OBJECT
public:
	ScaningCameraWidget( QWidget* parent);

	void show() override;

private slots:
	void barcodeObtained(QString);
signals:
	void hasBarcode(QString);
};
#endif
