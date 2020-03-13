#include "BarcodeRedactingWidget.h"
#include "widgets/utils/ElementsStyles.h"

BarcodeRedactingWidget::BarcodeRedactingWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	buttonLayout(new QHBoxLayout(this)),
	aboutForm(new QLabel(this)), aboutQuantity(new QLabel(this)),
	aboutExpDates(new QLabel(this)), aboutBarcode(new QLabel(this)),
	aboutComment(new QLabel(this)), quantity(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, this)),
	comment(new QTextEdit(this)), expDateedit(new QDateEdit(this)),
	expTimeEdit(new BigButtonsSpinbox(BigButtonsSpinbox::timespin, this)),
	okButton(new MegaIconButton(this)), backButton(new MegaIconButton(this)),
	removeButton(new MegaIconButton(this)), keyFilter(new filters::CaptureBackFilter(this)),
	entityCopy()
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(aboutForm);
	mainLayout->addWidget(aboutBarcode);
	mainLayout->addWidget(aboutQuantity);
	mainLayout->addWidget(quantity);
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

	aboutForm->setText(tr("barcode_redact_additional_info"));	//	TODO : translations
	aboutQuantity->setText(tr("barcode_redact_quantity"));
	aboutExpDates->setText(tr("barcode_redact_expdate"));
	aboutComment->setText(tr("barcode_redact_comment"));
	aboutQuantity->setAlignment(Qt::AlignCenter);
	aboutForm->setAlignment(Qt::AlignCenter);
	aboutComment->setAlignment(Qt::AlignCenter);
	aboutExpDates->setAlignment(Qt::AlignCenter);
	aboutComment->setAlignment(Qt::AlignCenter);
	aboutBarcode->setAlignment(Qt::AlignCenter);
	quantity->setMinimum(-1000000);
	quantity->setMaximum(1000000);
	okButton->setIcon(QIcon(":/res/submit.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	removeButton->setIcon(QIcon(":/res/deleteData.png"));

	expDateedit->setCalendarPopup(true);

	okButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	removeButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	expDateedit->setStyleSheet(LARGE_BUTTON_STYLESHEET);
	expDateedit->calendarWidget()->setStyleSheet(BETTER_CALENDAR_STYLESHEET());

	expDateedit->installEventFilter(keyFilter);
	comment->installEventFilter(keyFilter);

	QObject::connect(okButton, &QPushButton::clicked, this, &BarcodeRedactingWidget::okPressed);
	QObject::connect(backButton, &QPushButton::clicked, this, &BarcodeRedactingWidget::backPressed);
	QObject::connect(removeButton, &QPushButton::clicked, this, &BarcodeRedactingWidget::removePressed);
	QObject::connect(quantity, &BigButtonsSpinbox::backRequired, this, &BarcodeRedactingWidget::backPressed);
	QObject::connect(expTimeEdit, &BigButtonsSpinbox::backRequired, this, &BarcodeRedactingWidget::backPressed);
	QObject::connect(keyFilter, &filters::CaptureBackFilter::backRequired, this, &BarcodeRedactingWidget::backPressed);
}

void BarcodeRedactingWidget::setBarcode(const Entity& barc)
// This method is setting environment for changing barcode. Never use .show() before you set up barcode
{
	entityCopy.reset(barc->clone());
	switch (barc->myType())
	{
	case int(barcodeUtil::barcodetypes::uniformBc):
	{
		Barcode bc = upcastEntity<BarcodeEntity>(barc);
		quantity->setValue(bc->getEnumerable());
		comment->setText(bc->comment);
		expDateedit->setDate(bc->expDate.date());
		expTimeEdit->setTime(bc->expDate.time());
		aboutBarcode->setText(bc->barcode);
	}
	default:
		return;
	}
	
}

void BarcodeRedactingWidget::backPressed()
{
	emit backRequired();
}
void BarcodeRedactingWidget::okPressed()
{
	switch (entityCopy->myType())
	{
		case int(barcodeUtil::barcodetypes::uniformBc) :
		{
			Barcode barcodeCopy = upcastEntity<BarcodeEntity>(entityCopy);

			barcodeCopy->expDate = QDateTime(expDateedit->date(), expTimeEdit->time());
			barcodeCopy->comment = comment->toPlainText();
			barcodeCopy->quantity = quantity->value();
		}
		default:
			return;
	}
	emit barcodeCommited(entityCopy);
}
void BarcodeRedactingWidget::removePressed()
{
	entityCopy->invalidate();
	emit barcodeCommited(entityCopy);
}
void BarcodeRedactingWidget::setTimeFormat(const QString fm)
{
	expTimeEdit->setDisplayFormat(fm);
}