#include "BarcodeRedactingWidget.h"
#include "widgets/utils/ElementsStyles.h"


Abs_Redactor* generateRedactor(Modes mode, QWidget* parent)
{
	switch (modenamesLinker[mode]->myType())
	{
	case int(barcodeUtil::barcodetypes::uniformBc):
		return new BarcodeRedactorForm(parent);
	case int(barcodeUtil::barcodetypes::pricedBc) :
		return new PricedBarcodeRedactorForm(parent);
	default:
		return Q_NULLPTR;
	}
}


BarcodeRedactingWidget::BarcodeRedactingWidget(Modes mode, QWidget* parent)
	: inframedWidget(parent),
	mainLayout(new QGridLayout(this)),
	redactor(generateRedactor(mode, this)),
	okButton(new MegaIconButton(this)),
	backButton(new MegaIconButton(this)),
	entityCopy(),
	originalLink()
{
	mainLayout->addWidget(redactor, 0, 0, 1, 0);
	mainLayout->addWidget(okButton, 1, 0);
	mainLayout->addWidget(backButton, 1, 1);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	
	okButton->setIcon(QIcon(":/res/submit.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	
	okButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(okButton, &QPushButton::clicked, this, &BarcodeRedactingWidget::okPressed);
	QObject::connect(backButton, &QPushButton::clicked, this, &BarcodeRedactingWidget::backPressed);
#else
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(backPressed()));
#endif
}

bool BarcodeRedactingWidget::setBarcode(const Entity& barc)
// This method is setting environment for changing barcode. Never use .show() before you set up barcode
{
    entityCopy.clear();
    entityCopy = Entity(barc->clone());
	originalLink = barc;
	return redactor->setRedacted(entityCopy);
}

void BarcodeRedactingWidget::backPressed()
{
	emit backRequired();
}
void BarcodeRedactingWidget::okPressed()
{
	redactor->extractAndSave();
	emit barcodeCommited(originalLink, entityCopy);
	emit backRequired();
}
