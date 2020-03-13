#pragma once
#include "widgets/parents/AbstractCameraWidget.h"

/*
	This widget is supposed to work with camera. Current realisation uses QML for avoiding
	QCamera android bugs. Warning - due to qtcreator bugs DO NOT alter qml file with additional
	widgets. THIS IS NOT WORKING. VideoOutput widget is bugged and is ignoring all layouts and acnhors
	so any additional widget will be hidden under it. To make more interaction with camera -
	declare new QML-slots.

	TODO: Add Zbar barcode detecting.

	Update:
		Now widget is fully functional. Camera is working, images are saved
		in /storage/emulated/0/DCIM/Camera

	slots:
		makeImage() = makes an image by invoking captureRequired through Qt metacall system

	MAJOR UPDATE:
		Now this class is inheritor of more abstract widget. This widget specialization
		is to make images only. It has own CameraSurface qml file and does not scan for
		barcodes. This abstraction layer was added to allow easy inheriting for expanding features.

*/

class CameraWidget : public AbstractCameraWidget
{
	Q_OBJECT
private:
	MegaIconButton* makeImageButton;

public:
	CameraWidget(QWidget* parent);

	void show() override;

private slots:
	void makeImage();
	void backRequire();
};
