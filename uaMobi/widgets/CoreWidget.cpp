#include "CoreWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/PriceBranch/PriceBranchWidget.h"

#ifdef DEBUG
#include "debugtrace.h"
#endif

CoreWidget::CoreWidget(QWidget* parent)
	: QWidget(parent), abstractDynamicNode( new inframedWidget(this), new QVBoxLayout(this)),
	innerLayout(new QGridLayout(untouchable)),
	inventory(new IndexedButton(0, untouchable)), supplies(new IndexedButton(1, untouchable)),
	search(new IndexedButton(2,untouchable)), simple(new IndexedButton(3, untouchable)),
	camera(new IndexedButton(4, untouchable)), prices(new IndexedButton(5, untouchable)),
	controlPanel(new QHBoxLayout(untouchable)), exitButton(new IgnorantButton(untouchable)),
	settingsButton(new MegaIconButton(untouchable)), lock(false)
{
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	qobject_cast<QVBoxLayout*>(mainLayout)->setDirection(QBoxLayout::BottomToTop);
#ifdef Q_OS_WIN
	setFixedSize(imitatePhoneSize(0.66));
#else
	this->setMinimumSize(calculateAdaptiveSize(0.7));
	this->setMaximumSize(calculateAdaptiveSize(1));
	this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
#endif
	mainLayout->setSizeConstraint(QBoxLayout::SizeConstraint::SetMaximumSize);
	untouchable->setLayout(innerLayout);
	innerLayout->addWidget(inventory, 0, 0);
	innerLayout->addWidget(supplies, 0, 1);
	innerLayout->addWidget(search, 0, 2);
	innerLayout->addWidget(simple, 1, 0);
	innerLayout->addWidget(camera, 1, 1);
	innerLayout->addWidget(prices, 1, 2);

	innerLayout->addLayout(controlPanel, 2, 0, 2, 0);
	controlPanel->addWidget(settingsButton);
	controlPanel->addWidget(exitButton);
	
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

	settingsButton->setIcon(QIcon(":/res/settings.png"));
	exitButton->setIcon(QIcon(":/res/exit.png"));
	settingsButton->setMinimumHeight(calculateAdaptiveButtonHeight());
	exitButton->setMinimumHeight(calculateAdaptiveButtonHeight());

	QObject::connect(settingsButton, &QPushButton::pressed, this, &CoreWidget::settingsPressed);
	QObject::connect(exitButton, &QPushButton::pressed, this, &CoreWidget::exitPressed);
	QObject::connect(inventory, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(supplies, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(search, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(simple, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(camera, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(prices, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);

}
void CoreWidget::exitPressed()
{
	//detrace_METHCALL("ExitPressed")
	AppSettings->Save();
	this->close();
}
void CoreWidget::settingsPressed()
{
	// Checking that this branch has no more valid settings windows
	if (currentlyOpened != untouchable)
	{
		if (!currentlyOpened->giveSettings())
			_hideAndDeleteCurrent(untouchable);
	}
	else
	{
		_hideAndDeleteCurrent(new SettingsForm(this));
		QObject::connect(_upCO<SettingsForm>(), &SettingsForm::backRequired,
			this, &CoreWidget::hideCurrent);
	}
}

void CoreWidget::hideCurrent()
// This method is not only hiding widgets - it will delete entire branch if it must be hidden
{
	_hideAndDeleteCurrent(untouchable);
}

void CoreWidget::branchRequired(int number)
{
	switch (number)
	{
	case (mpw::mainPageWidgets::Supplies):
		_hideAndDeleteCurrent(new SuppliesWidget(this));
		QObject::connect(_upCO<SuppliesWidget>(), &SuppliesWidget::backRequired,
			this, &CoreWidget::hideCurrent);
		break;
	case (mpw::mainPageWidgets::Inventory):
		_hideAndDeleteCurrent(new InventoryWidget(this));
		QObject::connect(_upCO<InventoryWidget>(), &BranchRootWidget::backRequired,
			this, &CoreWidget::hideCurrent);
		break;
	case (mpw::mainPageWidgets::Simple):
		_hideAndDeleteCurrent(new SimpleBranchWidget(this));
		QObject::connect(_upCO<SimpleBranchWidget>(), &BranchRootWidget::backRequired,
			this, &CoreWidget::hideCurrent);
		break;
	case (mpw::mainPageWidgets::Camera):
		_hideAndDeleteCurrent(new CameraWidget(this));
		QObject::connect(_upCO<CameraWidget>(), &AbstractCameraWidget::backRequired,
			this, &CoreWidget::hideCurrent);
		break;
	case (mpw::mainPageWidgets::Search):
		_hideAndDeleteCurrent(new SearchWidget(this));
		QObject::connect(_upCO<SearchWidget>(), &SearchWidget::backRequired,
			this, &CoreWidget::hideCurrent);
		break;
	case (mpw::mainPageWidgets::Prices):
		_hideAndDeleteCurrent(new PriceBranchWidget(this));
		QObject::connect(_upCO<PriceBranchWidget>(), &BranchRootWidget::backRequired,
			this, &CoreWidget::hideCurrent);
		break;
	default:
		break;
	}
}


void CoreWidget::allocateLock()
{
	lock = true;
}

void CoreWidget::deallocateLock()
{
	lock = false;
}

void CoreWidget::handleBack()
{
}

IndexedButton::IndexedButton(int Index, QWidget* parent)
	: MegaIconButton(parent), index(Index)
{
	QObject::connect(this, &MegaIconButton::clicked, this, &IndexedButton::clickReceived);
}

void IndexedButton::clickReceived()
{
	emit iclicked(index);
}
