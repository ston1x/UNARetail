#include "VirtualBarcodeKeyboard.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/utils/ElementsStyles.h"

VirtualBarcodeKeyboard::VirtualBarcodeKeyboard(QWidget* parent)
	: inframedWidget(parent),
	mainLayout(new QGridLayout(this)), numberButtons(),
	eraseButton(new QPushButton(this)), clearButton(new QPushButton(this)),
	backButton(new QPushButton(this)), okButton(new QPushButton(this)),
	barcodeLine(new QLineEdit(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(barcodeLine, 0, 0, 2, 0);

	QSizePolicy bsp(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding); // button
	//																			size policy
	int row = 2;
	int col = 0;
	numberButtons.resize(10);
	for (int i = 0; i < 9; ++i)
	{
		numberButtons[i] = new QPushButton(this);
		numberButtons[i]->setSizePolicy(bsp);
		numberButtons[i]->setText(QString::number(i + 1));
		mainLayout->addWidget(numberButtons[i], row, col);
		QObject::connect(numberButtons[i], &QPushButton::pressed, this, &VirtualBarcodeKeyboard::numberPressed);
		++col;
		if (col == 3)
		{
			col = 0;
			++row;
		}
	}
	numberButtons[9] = new QPushButton(this);
	numberButtons[9]->setSizePolicy(bsp);
	numberButtons[9]->setText("0");
	QObject::connect(numberButtons[9], &QPushButton::pressed, this, &VirtualBarcodeKeyboard::numberPressed);
	mainLayout->addWidget(numberButtons[9], 5, 1);
	mainLayout->addWidget(clearButton, 5, 0);
	mainLayout->addWidget(eraseButton, 5, 2);
	mainLayout->addWidget(okButton, 6, 0, 1, 2);
	mainLayout->addWidget(backButton, 6, 2);

	clearButton->setText("C");
	eraseButton->setText("<<");
	okButton->setIcon(QIcon(":/res/submit.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	clearButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	eraseButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);

	clearButton->setSizePolicy(bsp);
	eraseButton->setSizePolicy(bsp);
	okButton->setMinimumHeight(calculateAdaptiveButtonHeight());
	backButton->setMinimumHeight(calculateAdaptiveButtonHeight());
	setFocus();
	QObject::connect(clearButton, &QPushButton::pressed, barcodeLine, &QLineEdit::clear);
	QObject::connect(eraseButton, &QPushButton::pressed, barcodeLine, &QLineEdit::backspace);
	QObject::connect(okButton, &QPushButton::pressed, this, &VirtualBarcodeKeyboard::okPressed);
	QObject::connect(backButton, &QPushButton::pressed, this, &VirtualBarcodeKeyboard::backPressed);
}

bool VirtualBarcodeKeyboard::isExpectingControl(int contr)
{
#ifdef DEBUG
	detrace_METHCALL("isEcpectingControl with " << contr);

#endif
	++contr;
	if (contr > 0 && contr < 10)
	{
		barcodeLine->setText(barcodeLine->text() + QString::number(contr));
		return true;
	}
	return false;
}

void VirtualBarcodeKeyboard::okPressed()
{
	emit valueConfirmed(barcodeLine->text());
	emit backRequired();
}

void VirtualBarcodeKeyboard::backPressed()
{
	emit backRequired();
}

void VirtualBarcodeKeyboard::setStartingText(QString& str, VirtualBarcodeKeyboard::inputphases phs)
{
	phase = phs;
	barcodeLine->clear();
	barcodeLine->setText(str);
}


void VirtualBarcodeKeyboard::numberPressed()
{
	auto buttonsent = sender();
	if (buttonsent != nullptr)
	{
		QPushButton* castedPointer = dynamic_cast<QPushButton*>(buttonsent);
		if (castedPointer == nullptr)
			return;
		barcodeLine->setText(barcodeLine->text() + castedPointer->text());
	}
}