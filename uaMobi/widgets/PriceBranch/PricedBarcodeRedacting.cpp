#include "PricedBarcodeRedacting.h"
#include "widgets/utils/ElementsStyles.h"

PricedBarcodeRedacting::PricedBarcodeRedacting(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	buttonLayout(new QHBoxLayout(this)),
	aboutGeneralPrice(new QLabel(this)), aboutExpDates(new QLabel(this)),
	aboutDiscountPrice(new QLabel(this)), aboutSecondaryPrice(new QLabel(this)),
	aboutUnnecessaryPrice(new QLabel(this)),
	aboutComment(new QLabel(this)), generalPrice(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, this, 0.08)),
	discountPrice(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, this, 0.08)),
	secondaryPrice(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, this, 0.08)),
	unnecessaryPrice(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, this, 0.08)),
	comment(new QLineEdit(this)), expDateedit(new QDateEdit(this)),
	expTimeEdit(new BigButtonsSpinbox(BigButtonsSpinbox::timespin, this, 0.08)),
	okButton(new MegaIconButton(this)), backButton(new MegaIconButton(this)),
	removeButton(new MegaIconButton(this)), keyFilter(new filters::CaptureBackFilter(this)),
	barcodeCopy()
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(aboutGeneralPrice);
	mainLayout->addWidget(generalPrice);
	mainLayout->addWidget(aboutDiscountPrice);
	mainLayout->addWidget(discountPrice);
	mainLayout->addWidget(aboutSecondaryPrice);
	mainLayout->addWidget(secondaryPrice);
	mainLayout->addWidget(aboutUnnecessaryPrice);
	mainLayout->addWidget(unnecessaryPrice);
	mainLayout->addWidget(aboutExpDates);
	mainLayout->addWidget(expDateedit);
	mainLayout->addWidget(expTimeEdit);
	mainLayout->addWidget(aboutComment);
	mainLayout->addWidget(comment);
	mainLayout->addStretch();
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(removeButton);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	aboutGeneralPrice->setText(tr("barcode_redact_general_desc"));	//	TODO : translations
	aboutDiscountPrice->setText(tr("priced_barcode_discount_desc"));
	aboutSecondaryPrice->setText(tr("priced_barcode_secondary_desc"));
	aboutUnnecessaryPrice->setText(tr("priced_barcode_redact_unnec_desc"));
	aboutExpDates->setText(tr("barcode_redact_expdate"));
	aboutComment->setText(tr("barcode_redact_comment"));
	aboutGeneralPrice->setAlignment(Qt::AlignCenter);
	aboutDiscountPrice->setAlignment(Qt::AlignCenter);
	aboutComment->setAlignment(Qt::AlignCenter);
	aboutExpDates->setAlignment(Qt::AlignCenter);
	aboutComment->setAlignment(Qt::AlignCenter);
	aboutUnnecessaryPrice->setAlignment(Qt::AlignCenter);
	aboutSecondaryPrice->setAlignment(Qt::AlignCenter);
	generalPrice->setMinimum(0);
	generalPrice->setMaximum(1000000);
	secondaryPrice->setMinimum(0);
	secondaryPrice->setMaximum(1000000);
	discountPrice->setMinimum(0);
	discountPrice->setMaximum(1000000);
	unnecessaryPrice->setMinimum(0);
	unnecessaryPrice->setMaximum(1000000);

	okButton->setIcon(QIcon(":/res/submit.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	removeButton->setIcon(QIcon(":/res/deleteData.png"));

	expDateedit->setCalendarPopup(true);
	expDateedit->setMinimumHeight(calculateAdaptiveButtonHeight(0.09));

	okButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	removeButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	expDateedit->setStyleSheet(LARGE_BUTTON_STYLESHEET);
	expDateedit->calendarWidget()->setStyleSheet(BETTER_CALENDAR_STYLESHEET());

	expDateedit->installEventFilter(keyFilter);
	comment->installEventFilter(keyFilter);

	QObject::connect(okButton, &QPushButton::clicked, this, &PricedBarcodeRedacting::okPressed);
	QObject::connect(backButton, &QPushButton::clicked, this, &PricedBarcodeRedacting::backPressed);
	QObject::connect(removeButton, &QPushButton::clicked, this, &PricedBarcodeRedacting::removePressed);
	QObject::connect(generalPrice, &BigButtonsSpinbox::backRequired, this, &PricedBarcodeRedacting::backPressed);
	QObject::connect(discountPrice, &BigButtonsSpinbox::backRequired, this, &PricedBarcodeRedacting::backPressed);
	QObject::connect(secondaryPrice, &BigButtonsSpinbox::backRequired, this, &PricedBarcodeRedacting::backPressed);
	QObject::connect(unnecessaryPrice, &BigButtonsSpinbox::backRequired, this, &PricedBarcodeRedacting::backPressed);
	QObject::connect(expTimeEdit, &BigButtonsSpinbox::backRequired, this, &PricedBarcodeRedacting::backPressed);
	QObject::connect(keyFilter, &filters::CaptureBackFilter::backRequired, this, &PricedBarcodeRedacting::backPressed);
}

void PricedBarcodeRedacting::setBarcode(const Entity barc)
{
	barcodeCopy.reset(barc->clone());
	switch (barc->myType())
	{
	case int(barcodeUtil::barcodetypes::pricedBc) :
	{
		PricedBarcode bc = upcastEntity<PricedBarcodeEntity>(barc);
		generalPrice->setValue(bc->generalPrice);
		discountPrice->setValue(bc->discountPrice);
		secondaryPrice->setValue(bc->secondaryPrice);
		unnecessaryPrice->setValue(bc->unnecessaryPrice);
		comment->setText(bc->comment);
		expDateedit->setDate(bc->expDate.date());
		expTimeEdit->setTime(bc->expDate.time());
	}
	default:
		break;
	}
}

void PricedBarcodeRedacting::setTimeFormat(const QString fm)
{
	expTimeEdit->setDisplayFormat(fm);
}

void PricedBarcodeRedacting::okPressed()
{
	if (barcodeCopy != Q_NULLPTR && barcodeCopy->myType() == int(barcodeUtil::barcodetypes::pricedBc))
	{
		PricedBarcode bc = upcastEntity<PricedBarcodeEntity>( barcodeCopy);
		bc->expDate = QDateTime(expDateedit->date(), expTimeEdit->time());
		bc->comment = comment->text();
		bc->generalPrice = generalPrice->value();
		bc->discountPrice = discountPrice->value();
		bc->secondaryPrice = secondaryPrice->value();
		bc->unnecessaryPrice = unnecessaryPrice->value();
	}
	emit barcodeCommited(barcodeCopy);
}

void PricedBarcodeRedacting::backPressed()
{
	emit backRequired();
}

void PricedBarcodeRedacting::removePressed()
{
	if (barcodeCopy != Q_NULLPTR)
	{
		barcodeCopy->invalidate();
		emit barcodeCommited(barcodeCopy);
	}
}