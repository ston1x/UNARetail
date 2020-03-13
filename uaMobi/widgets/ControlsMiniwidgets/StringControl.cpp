#include "StringControl.h"
#include "widgets/utils/ElementsStyles.h"


QString StringControl::prepareAndReturnValue() const
{
	return input->text();
}

bool StringControl::parseAndSetValue(QString str)
{
	input->setText(str);
	return true;
}

void StringControl::clear()
{
	input->clear();
}

bool StringControl::valueAvailable() const
{
	return !input->text().isEmpty();
}

QWidget* StringControl::getInnerWidget() const
{
	return innerWidget;
}

StringControl::StringControl(QString Name, QWidget* Parent)
    : abs_control(Parent, InputControlEntity::String, Name),
	innerWidget(new QWidget(parent)),
	innerLayout(new QVBoxLayout(innerWidget)),
	about(new QLabel(innerWidget)),
	input(new QLineEdit(innerWidget))
{
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(about);
	innerLayout->addWidget(input);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	about->setAlignment(Qt::AlignCenter);
	about->setFont(FontAdapter::makeFont(1.2));
	about->setText(label);
	input->setAlignment(Qt::AlignCenter);
	input->setFont(FontAdapter::makeFont(1.3));
#ifdef QT_VERSION5X
	QObject::connect(input, &QLineEdit::returnPressed, this, &StringControl::subcontrolEditingFinished);
#else
	QObject::connect(input, SIGNAL(returnPressed()), this, SLOT(subcontrolEditingFinished()));
#endif
}

StringControl::~StringControl()
{
	innerWidget->deleteLater();
}

void StringControl::setFocus() const
{
	input->setFocus();
}

void StringControl::show()
{
	innerWidget->show();
}

void StringControl::hide()
{
	innerWidget->hide();
}
