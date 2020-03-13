#include "ScaningCameraWidget.h"
#include <QtQuick/QQuickItem>
void ScaningCameraWidget::show()
{
	inframedWidget::show();
}
void ScaningCameraWidget::barcodeObtained(QString str)
{
	backButton->setText(str);
	emit hasBarcode(str);
	emit backRequired();
}
ScaningCameraWidget::ScaningCameraWidget(QWidget* parent)
	: AbstractCameraWidget("qrc:///ScaningCameraSurface.qml", parent)
{
	auto root2 = qmlCamera->rootObject();
	auto filter = root2->findChild<QObject*>("FilterZX");
	QObject::connect(filter, SIGNAL(bcArr(QString)), this, SLOT(barcodeObtained(QString)));
}