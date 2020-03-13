#include "AbstractCameraWidget.h"
#include <QQMLError>
#include <QtQuick/qquickitem.h>
#include <widgets/utils/ElementsStyles.h>

AbstractCameraWidget::AbstractCameraWidget(QString qmlcamPath, QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	innerWidget(new QWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	buttonLayout(new QHBoxLayout(innerWidget)), qmlCamera(new QQuickView()), //qmlCamera(new QQuickWidget(innerWidget)),
	viewHolder(),
	backButton(new MegaIconButton(innerWidget))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	viewHolder = QWidget::createWindowContainer(qmlCamera);
	innerLayout->addWidget(viewHolder);
	innerLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	mainLayout->setSpacing(0);
	innerLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setDirection(QBoxLayout::RightToLeft);

	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	qmlCamera->setSource(QUrl(qmlcamPath));
	qmlCamera->setResizeMode(QQuickView::ResizeMode::SizeRootObjectToView);
	// qmlCamera->resize(innerWidget->size());
	update();

	QObject::connect(backButton, &QPushButton::pressed, this, &AbstractCameraWidget::backRequire);
}

void AbstractCameraWidget::backRequire()
{
	emit backRequired();
}