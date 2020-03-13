#include "MainPageWidget.h"
#include "widgets/utils/ElementsStyles.h"

MainPageWidget::MainPageWidget( QWidget* parent)
	: inframedWidget(parent), mainLayout(new QGridLayout(this)),
	inventory(new MegaIconButton(this)), supplies(new MegaIconButton(this)),
	search(new MegaIconButton(this)), simple(new MegaIconButton(this)),
	camera(new MegaIconButton(this)), prices(new MegaIconButton(this))
{
	this->setLayout(mainLayout);

	mainLayout->addWidget(inventory, 0, 0);
	mainLayout->addWidget(supplies, 0, 1);
	mainLayout->addWidget(search, 0, 2);
	mainLayout->addWidget(simple, 1, 0);
	mainLayout->addWidget(camera, 1, 1);
	mainLayout->addWidget(prices, 1, 2);
	mainLayout->setSpacing(0);				//	Spacing removed to avoid space loss
	mainLayout->setContentsMargins(0, 0, 0, 0);

	inventory->setIcon(QIcon(":/res/inventory.png"));
	supplies->setIcon(QIcon(":/res/rec.png"));
	search->setIcon(QIcon(":/res/find.png"));
	simple->setIcon(QIcon(":/res/pen.png"));
	camera->setIcon(QIcon(":/res/cameradoc.png"));
	prices->setIcon(QIcon(":/res/cash.png"));

	inventory->setText(tr("inventory"));
	supplies->setText(tr("supplies"));
	search->setText(tr("search"));
	simple->setText(tr("simple"));
	camera->setText(tr("camera"));
	prices->setText(tr("prices_mode"));

	QSizePolicy sizePol(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	this->setSizePolicy(sizePol);
	inventory->setSizePolicy(sizePol);
	supplies->setSizePolicy(sizePol);
	search->setSizePolicy(sizePol);
	simple->setSizePolicy(sizePol);
	camera->setSizePolicy(sizePol);
	prices->setSizePolicy(sizePol);

	QObject::connect(inventory, &QPushButton::pressed, this, &MainPageWidget::inventoryPressed);
	QObject::connect(supplies, &QPushButton::pressed, this, &MainPageWidget::suppliesPressed);
	QObject::connect(search, &QPushButton::pressed, this, &MainPageWidget::searchPressed);
	QObject::connect(simple, &QPushButton::pressed, this, &MainPageWidget::simplePressed);
	QObject::connect(camera, &QPushButton::pressed, this, &MainPageWidget::cameraPressed);
	QObject::connect(prices, &QPushButton::pressed, this, &MainPageWidget::pricesPressed);
}

bool MainPageWidget::back()
{
	return false;
}

bool MainPageWidget::giveSettings()
{
	return false;
}
bool MainPageWidget::isExpectingControl(int contr)
{
	if (contr >= int(mpw::mainPageWidgets::Inventory) && contr <= int(mpw::mainPageWidgets::Prices))
	{
		emit widgetSelected((mpw::mainPageWidgets)contr);
	}
	return true;
}
void MainPageWidget::suppliesPressed()
{
	emit widgetSelected(mpw::mainPageWidgets::Supplies);
}

void MainPageWidget::searchPressed()
{
	emit widgetSelected(mpw::mainPageWidgets::Search);
}
void MainPageWidget::pricesPressed()
{
	emit widgetSelected(mpw::mainPageWidgets::Prices);
}

void MainPageWidget::simplePressed()
{
	emit widgetSelected(mpw::mainPageWidgets::Simple);
}

void MainPageWidget::cameraPressed()
{
	emit widgetSelected(mpw::mainPageWidgets::Camera);
}
void MainPageWidget::inventoryPressed()
{
	emit widgetSelected(mpw::mainPageWidgets::Inventory);
}
void MainPageWidget::changeEvent(QEvent* pe)
{
	if (pe->type() == QEvent::LanguageChange)
	{
		inventory->setText(tr("inventory"));
		supplies->setText(tr("supplies"));
		search->setText(tr("search"));
		simple->setText(tr("simple"));
		camera->setText(tr("camera"));
		prices->setText(tr("prices_mode"));
	}
}