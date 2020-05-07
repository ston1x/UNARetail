#include "CameraWidget.h"
#ifdef CAMERA_SUPPORT
#include "widgets/utils/ElementsStyles.h"
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>

CameraWidget::CameraWidget(QWidget* parent)
	: AbstractCameraWidget("qrc:///CameraSurface.qml", parent),
	makeImageButton(new MegaIconButton(innerWidget))
{
	buttonLayout->addWidget(makeImageButton);
	makeImageButton->setIcon(QIcon(":/res/new.png"));

	makeImageButton->setStyleSheet(OK_BUTTONS_STYLESHEET);

	QObject::connect(makeImageButton, &QPushButton::pressed, this, &CameraWidget::makeImage);
}

void CameraWidget::show()
{
	inframedWidget::show();
}

void CameraWidget::makeImage()
{
	QObject* camera = qmlCamera->rootObject()->findChild<QObject*>("Maincamera");
	QMetaObject::invokeMethod(camera, "onCaptureRequired");
}

void CameraWidget::backRequire()
{
	emit backRequired();
}
#endif