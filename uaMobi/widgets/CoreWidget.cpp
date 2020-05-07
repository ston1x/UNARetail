#include "CoreWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/PriceBranch/PriceBranchWidget.h"
#include "widgets/InvoiceBranch/InvoiceBranchWidget.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

CoreWidget::CoreWidget(QWidget* parent)
	: QWidget(parent), abstractDynamicNode( new inframedWidget(this), new QVBoxLayout(this)),
	innerLayout(new QGridLayout(untouchable)),
	inventory(new IndexedButton(0, untouchable)), supplies(new IndexedButton(1, untouchable)),
	search(new IndexedButton(2,untouchable)), simple(new IndexedButton(3, untouchable)),
	invoice(new IndexedButton(4, untouchable)), 
	prices(new IndexedButton(5, untouchable)),
	controlPanel(new QHBoxLayout(untouchable)), exitButton(new IgnorantButton(untouchable)),
	settingsButton(new MegaIconButton(untouchable)), lock(false)
{
	
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	qobject_cast<QVBoxLayout*>(mainLayout)->setDirection(QBoxLayout::BottomToTop);
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	setFixedSize(imitatePhoneSize(0.66));
#endif
#else
	this->setMinimumSize(calculateAdaptiveSize(0.7));
	this->setMaximumSize(calculateAdaptiveSize(1));
	this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
#endif
	untouchable->setLayout(innerLayout);
	innerLayout->addWidget(inventory, 0, 0);
	innerLayout->addWidget(supplies, 0, 1);
	innerLayout->addWidget(search, 0, 2);
	innerLayout->addWidget(prices, 1, 2);
	innerLayout->addWidget(simple, 1, 0);
	innerLayout->addWidget(invoice, 1, 1);
	setFont(AppGenFont);
	innerLayout->addLayout(controlPanel, 2, 0, 2, 0);
	controlPanel->addWidget(settingsButton);
	controlPanel->addWidget(exitButton);
	
	
	inventory->setIcon(QIcon(":/res/inventory.png"));
	supplies->setIcon(QIcon(":/res/rec.png"));
	search->setIcon(QIcon(":/res/find.png"));
	simple->setIcon(QIcon(":/res/pen.png"));
	invoice->setIcon(QIcon(":/res/best.png"));
	prices->setIcon(QIcon(":/res/cash.png"));

	inventory->setText(tr("inventory"));
	supplies->setText(tr("supplies"));
	search->setText(tr("search"));
	simple->setText(tr("simple"));
	invoice->setText(tr("invoice"));
	prices->setText(tr("prices_mode"));


	QSizePolicy sizePol(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
	inventory->setSizePolicy(sizePol);
	supplies->setSizePolicy(sizePol);
	search->setSizePolicy(sizePol);
	simple->setSizePolicy(sizePol);
	prices->setSizePolicy(sizePol);
	invoice->setSizePolicy(sizePol);

	settingsButton->setIcon(QIcon(":/res/settings.png"));
	exitButton->setIcon(QIcon(":/res/exit.png"));
	settingsButton->setMinimumHeight(calculateAdaptiveButtonHeight());
	exitButton->setMinimumHeight(calculateAdaptiveButtonHeight());
	inventory->setMaximumWidth(calculateAdaptiveWidth(0.333));
	supplies->setMaximumWidth(calculateAdaptiveWidth(0.333));
#ifdef QT_VERSION5X
	QObject::connect(settingsButton, &QPushButton::clicked, this, &CoreWidget::settingsPressed);
	QObject::connect(exitButton, &QPushButton::clicked, this, &CoreWidget::exitPressed);
	QObject::connect(inventory, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(supplies, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(search, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(simple, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(prices, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
	QObject::connect(invoice, &IndexedButton::iclicked, this, &CoreWidget::branchRequired);
#else
	QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(settingsPressed()));
	QObject::connect(exitButton, SIGNAL(clicked()), this, SLOT(exitPressed()));
	QObject::connect(inventory, SIGNAL(iclicked(int)), this, SLOT(branchRequired(int)));
	QObject::connect(supplies, SIGNAL(iclicked(int)), this, SLOT(branchRequired(int)));
	QObject::connect(search, SIGNAL(iclicked(int)), this, SLOT(branchRequired(int)));
	QObject::connect(simple, SIGNAL(iclicked(int)), this, SLOT(branchRequired(int)));
	QObject::connect(prices, SIGNAL(iclicked(int)), this, SLOT(branchRequired(int)));
#endif

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
#ifdef QT_VERSION5X
		QObject::connect(_upCO<SettingsForm>(), &SettingsForm::backRequired,
			this, &CoreWidget::hideCurrent);
		QObject::connect(_upCO<SettingsForm>(), &SettingsForm::retranslated,
			this, &CoreWidget::retranslate);
		QObject::connect(_upCO<SettingsForm>(), &SettingsForm::fontsChanged,
			this, &CoreWidget::refreshFonts);
#else
		QObject::connect(_upCO<SettingsForm>(), SIGNAL(backRequired()),
			this, SLOT(hideCurrent()));
		QObject::connect(_upCO<SettingsForm>(), SIGNAL(retranslated()),
			this, SLOT(retranslate()));
		QObject::connect(_upCO<SettingsForm>(), SIGNAL(fontsChanged()),
			this, SLOT(refreshFonts()));
#endif

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
		break;
	case (mpw::mainPageWidgets::Inventory):
		_hideAndDeleteCurrent(new InventoryWidget(this));
	
		break;
	case (mpw::mainPageWidgets::Simple):
		_hideAndDeleteCurrent(new SimpleBranchWidget(this));
		break;
	case (mpw::mainPageWidgets::Invoice):
		_hideAndDeleteCurrent(new InvoiceBranchWidget(this));
		break;
	case (mpw::mainPageWidgets::Search):
		_hideAndDeleteCurrent(new SearchWidget(this));
		break;
	case (mpw::mainPageWidgets::Prices):
		_hideAndDeleteCurrent(new PriceBranchWidget(this));
		break;
	default:
		break;
	}
	if (currentlyOpened != Q_NULLPTR)
	{
#ifdef QT_VERSION5X
		QObject::connect(currentlyOpened, &inframedWidget::backRequired,
			this, &CoreWidget::hideCurrent);
#else
		QObject::connect(currentlyOpened, SIGNAL(backRequired()),
			this, SLOT(hideCurrent()));
#endif
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

void CoreWidget::retranslate()
{
	inventory->setText(tr("inventory"));
	supplies->setText(tr("supplies"));
	search->setText(tr("search"));
	simple->setText(tr("simple"));
	invoice->setText(tr("invoice"));
	prices->setText(tr("prices_mode"));
}

void CoreWidget::refreshFonts()
{
	setFont(AppGenFont);
}

IndexedButton::IndexedButton(int Index, QWidget* parent)
	: MegaIconButton(parent), index(Index)
{
#ifdef QT_VERSION5X
	QObject::connect(this, &MegaIconButton::clicked, this, &IndexedButton::clickReceived);
#else
	QObject::connect(this, SIGNAL(clicked()), this, SLOT(clickReceived()));
#endif
}

void IndexedButton::clickReceived()
{
	emit iclicked(index);
}
