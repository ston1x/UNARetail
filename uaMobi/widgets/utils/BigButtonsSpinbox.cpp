#include "BigButtonsSpinbox.h"
#include "widgets/utils/ElementsStyles.h"
#include <cmath>
#include <qcalendarwidget.h>

#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromUtf8("" A "" , sizeof(A)-1)
#endif

void BigButtonsSpinbox::showEvent(QShowEvent* ev)
{
	QWidget::showEvent(ev);
	if (sptype == datespin)
	{
		ReturnReactingDateEdit* tsp = qobject_cast<ReturnReactingDateEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			QString tmp = "ReturnReactingDateEdit{"
				"border-radius: 1px;"
				"padding: 1px 1px 1px 1px;"
				"border: 1px solid black;"
				"}"
				"ReturnReactingDateEdit::drop-down {"
				"subcontrol-origin: padding;"
				"subcontrol-position: center right;"
                "width: " + QString::number(int(coreSpinbox->width() / 3)) + " px;"
				"}"
				;
			tsp->setStyleSheet(tmp);
		}
	}
}
BigButtonsSpinbox::BigButtonsSpinbox(spintype type, QWidget* parent, double adaptH)
	: QWidget(parent), mainLayout(new QGridLayout(this)),
	buttonUp(new QPushButton(this)), buttonDown(new QPushButton(this)),
    infoLabel(new QLabel(this)),
	coreSpinbox(),
    keyFilter(new filters::CaptureBackFilter(this)),
	lastEmit(QTime::currentTime())
{
	switch (type)
	{
	case intspin:
		coreSpinbox = new ReturnReactingSpinBox(this);
		break;
	case timespin:
		coreSpinbox = new ReturnReactingTimeEdit(this);
		break;
	case floatspin:
		coreSpinbox = new ReturnReactingDoubleSpinBox(this);
		break;
	case datespin:
		coreSpinbox = new ReturnReactingDateEdit(this);
		break;
	case datetimespin:
		coreSpinbox = new ReturnReactingDateTimeEdit(this);
	}
	sptype = type;
	this->setLayout(mainLayout);
	mainLayout->addWidget(buttonUp, 0, 0, 3, 1);
	mainLayout->addWidget(infoLabel, 0, 1);
	mainLayout->addWidget(coreSpinbox, 1, 1, 2, 1);
	mainLayout->addWidget(buttonDown, 0, 2, 3, 1);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);


	coreSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	coreSpinbox->setFixedHeight(calculateAdaptiveButtonHeight(adaptH * 0.6666));
	coreSpinbox->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));
	coreSpinbox->installEventFilter(keyFilter);
	coreSpinbox->setAlignment(Qt::AlignCenter);
	coreSpinbox->setFont(AppFonts->makeFont(3.0));
	if (sptype == datespin)
	{
		buttonUp->hide();
		buttonDown->hide();
	}
	else
	{
		buttonUp->setIcon(QIcon(":/res/uparrow.png"));
		buttonDown->setIcon(QIcon(":/res/downarrow.png"));
		buttonUp->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
		buttonDown->setMinimumHeight(calculateAdaptiveButtonHeight(adaptH));
		buttonDown->setFocusPolicy(Qt::NoFocus);
		buttonUp->setFocusPolicy(Qt::NoFocus);
		buttonUp->setStyleSheet(UP_SPINBOX_STYLESHEET);
		buttonDown->setStyleSheet(DOWN_SPINBOX_STYLESHEET);
	}
	infoLabel->setFixedHeight(calculateAdaptiveButtonHeight(adaptH / 3));
	infoLabel->setFont(AppFonts->makeFont(0.45));
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setStyleSheet("QLabel{border: 1px solid black;}");

#ifdef QT_VERSION5X
	QObject::connect(buttonUp, &QPushButton::pressed, coreSpinbox, &QAbstractSpinBox::stepUp);
	QObject::connect(buttonDown, &QPushButton::pressed, coreSpinbox, &QAbstractSpinBox::stepDown);
	QObject::connect(keyFilter, &filters::CaptureBackFilter::backRequired, this, &BigButtonsSpinbox::backRequire);
#else
	QObject::connect(buttonUp, SIGNAL(pressed()), coreSpinbox, SLOT(stepUp()));
	QObject::connect(buttonDown, SIGNAL(pressed()), coreSpinbox, SLOT(stepDown()));
	QObject::connect(keyFilter, SIGNAL(backRequired()), this, SLOT(backRequire()));
#endif

	switch (sptype)
		//RTTI used to connect right signals
	{
	case intspin:
	{
		ReturnReactingSpinBox* isp;
		isp = qobject_cast<ReturnReactingSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR) {
			isp->setSpecialValueText("");
#ifdef QT_VERSION5X
			QObject::connect(isp, QOverload<int>::of(&ReturnReactingSpinBox::valueChanged), this, &BigButtonsSpinbox::intValueChanged);
			QObject::connect(isp, &ReturnReactingSpinBox::returnPressed, this, &BigButtonsSpinbox::returnPressed);
#else
			QObject::connect(isp, SIGNAL(valueChanged(int)), this, SLOT(intValueChanged(int)));
			QObject::connect(isp, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
#endif
		}
		break;
	}
	case floatspin:
	{
		ReturnReactingDoubleSpinBox* dsp;
		dsp = qobject_cast<ReturnReactingDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setSpecialValueText("");
            dsp->setDecimals(3);
#ifdef QT_VERSION5X
			QObject::connect(dsp, QOverload<double>::of(&ReturnReactingDoubleSpinBox::valueChanged), this, &BigButtonsSpinbox::doubleValueChanged);
			QObject::connect(dsp, &ReturnReactingDoubleSpinBox::returnPressed, this, &BigButtonsSpinbox::returnPressed);
#else
			QObject::connect(dsp, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
			QObject::connect(dsp, SIGNAL(valueChanged(double)), this, SLOT(doubleValueChanged(double)));
#endif
		}
		break;
	}
	case datespin:
	{
		ReturnReactingDateEdit* dsp;
		dsp = qobject_cast<ReturnReactingDateEdit*>(coreSpinbox);
		{
			if (dsp != Q_NULLPTR)
			{
				dsp->setSpecialValueText("");
				dsp->setDate(QDate::currentDate());
				dsp->setCalendarPopup(true);
				dsp->setDisplayFormat(QStringLiteral("dd:MM:yyyy")); 
#ifdef QT_VERSION5X
				QObject::connect(dsp, &ReturnReactingDateEdit::dateChanged, this, &BigButtonsSpinbox::dateChanged);
				QObject::connect(dsp, &ReturnReactingDateEdit::returnPressed, this, &BigButtonsSpinbox::returnPressed);
#else
                QObject::connect(dsp, SIGNAL(dateChanged(const QDate&)), this, SIGNAL(dateChanged(const QDate&)));
				QObject::connect(dsp, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
#endif
			}
		}
		break;
	}
	case timespin:
	{
		ReturnReactingTimeEdit* tsp;
		tsp = qobject_cast<ReturnReactingTimeEdit*>(coreSpinbox);
		{
			if (tsp != Q_NULLPTR)
			{
				tsp->setSpecialValueText("");
				tsp->setTime(QTime::currentTime());
				tsp->setDisplayFormat(QStringLiteral("HH:mm"));
#ifdef QT_VERSION5X
				QObject::connect(tsp, &ReturnReactingTimeEdit::timeChanged, this, &BigButtonsSpinbox::timeChanged);
				QObject::connect(tsp, &ReturnReactingTimeEdit::returnPressed, this, &BigButtonsSpinbox::returnPressed);
#else
                QObject::connect(tsp, SIGNAL(timeChanged(const QTime&)), this, SIGNAL(timeChanged(const QTime&)));
				QObject::connect(tsp, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
#endif
			}
		}
		break;
	}
	case datetimespin:
		ReturnReactingDateTimeEdit* dtp;
		dtp = qobject_cast<ReturnReactingDateTimeEdit*>(coreSpinbox);
		{
			if (dtp != Q_NULLPTR)
			{
				dtp->setSpecialValueText("");
				dtp->setDateTime(QDateTime::currentDateTime());
				dtp->setCalendarPopup(true);
				dtp->setDisplayFormat(QStringLiteral("dd:MM:yyyy"));
#ifdef QT_VERSION5X
				QObject::connect(dtp, &ReturnReactingDateTimeEdit::dateTimeChanged, this, &BigButtonsSpinbox::dateTimeChanged);
				QObject::connect(dtp, &ReturnReactingDateTimeEdit::returnPressed, this, &BigButtonsSpinbox::returnPressed);
#else
                QObject::connect(dtp, SIGNAL(dateTimeChanged(const QDateTime&)), this, SIGNAL(dateTimeChanged(const QDateTime&)));
				QObject::connect(dtp, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
#endif
			}
		}
        break;
	}

}

void BigButtonsSpinbox::setMinimum(int min)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		ReturnReactingSpinBox* isp = qobject_cast<ReturnReactingSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setMinimum(min);
		}
		break;
	}
	case floatspin:
	{
		ReturnReactingDoubleSpinBox* dsp = qobject_cast<ReturnReactingDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setMinimum(min);
		}
		break;
	}
    default:
        break;
	}
}

void BigButtonsSpinbox::setMaximum(int max)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		ReturnReactingSpinBox* isp = qobject_cast<ReturnReactingSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setMaximum(max);
		}
		break;
	}
	case floatspin:
	{
		ReturnReactingDoubleSpinBox* dsp = qobject_cast<ReturnReactingDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setMaximum(max);
		}
		break;
	}
    default:break;
	}
}

void BigButtonsSpinbox::setValue(int val)
{
	switch (sptype)
	{		// }
	case intspin:
	{
		ReturnReactingSpinBox* isp = qobject_cast<ReturnReactingSpinBox*>(coreSpinbox);	// <-This is normal RTTI check:
		if (isp != Q_NULLPTR)							//	to see if this pointer can be casted
		{								//}
			isp->setValue(val);
		}
		break;
	}
	case floatspin:
	{
		ReturnReactingDoubleSpinBox* dsp = qobject_cast<ReturnReactingDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setValue(val);
		}
		break;
	}
    default:
        break;
	}
}

void BigButtonsSpinbox::setDValue(double val)
{
	switch (sptype) {
	case floatspin:
	{
		ReturnReactingDoubleSpinBox* dsp = qobject_cast<ReturnReactingDoubleSpinBox*>(coreSpinbox);
		if (dsp != Q_NULLPTR)
		{
			dsp->setValue(val);
		}
		break;
	}
	case intspin:
	{
		ReturnReactingSpinBox* isp = qobject_cast<ReturnReactingSpinBox*> (coreSpinbox);
		if (isp != Q_NULLPTR)
		{
            int floored = int(std::ceil(val));
			isp->setValue(floored);
		}
		break;
	}
    default:
        break;
	}
}

void BigButtonsSpinbox::setTime(const QTime& tm)
{
	if (sptype == timespin)
	{
		ReturnReactingTimeEdit* tsp = qobject_cast<ReturnReactingTimeEdit*> (coreSpinbox);
        if (tsp != Q_NULLPTR)
		{
			tsp->setTime(tm);
		}
	}
}

int BigButtonsSpinbox::value() const
{
	switch (sptype)
	{
	case intspin:
	{	ReturnReactingSpinBox* isp = qobject_cast<ReturnReactingSpinBox*>(coreSpinbox);
	if (isp != Q_NULLPTR)
	{
		return isp->value();
	}
	break;
	}
	case floatspin:
	{	ReturnReactingDoubleSpinBox* dsp = qobject_cast<ReturnReactingDoubleSpinBox*> (coreSpinbox);
	if (dsp != Q_NULLPTR)
	{
        return int(dsp->value());
	}
	break;
	}
	default:
		break;
	}
	return 0;
}

double BigButtonsSpinbox::dvalue() const
{
	switch (sptype)
	{
	case floatspin:
	{	ReturnReactingDoubleSpinBox* dsp = qobject_cast<ReturnReactingDoubleSpinBox*>(coreSpinbox);
	if (dsp != Q_NULLPTR)
	{
		return dsp->value();
	}
	break; }
	case intspin:
	{
		ReturnReactingSpinBox* isp = qobject_cast<ReturnReactingSpinBox*>(coreSpinbox);
		if (isp != Q_NULLPTR)
		{
            return double(isp->value());
		}
		break;
	}
    default:
        break;
	}
	return 0.0;
}

QTime BigButtonsSpinbox::time()
{
	if (sptype == timespin)
	{
		ReturnReactingTimeEdit* tsp = qobject_cast<ReturnReactingTimeEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			return tsp->time();
		}
	}
	return QTime();
}

void BigButtonsSpinbox::setDisplayFormat(const QString& tf)
{
	if (sptype == timespin)
	{
		ReturnReactingTimeEdit* tsp = qobject_cast<ReturnReactingTimeEdit*>(coreSpinbox);
		if (tsp != Q_NULLPTR)
		{
			tsp->setDisplayFormat(tf);
		}
	}
}

QDate BigButtonsSpinbox::getDate()
{
	if (sptype == datespin || sptype == datetimespin)
	{
		ReturnReactingDateTimeEdit* de = qobject_cast<ReturnReactingDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			return de->date();
	}
	return QDate();
}

void BigButtonsSpinbox::setDate(const QDate& dt)
{
	if (sptype == datespin || sptype == datetimespin)
	{
		ReturnReactingDateTimeEdit* de = qobject_cast<ReturnReactingDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			de->setDate(dt);
	}
}

QDateTime BigButtonsSpinbox::getDateTime()
{
	if (sptype == datespin || sptype == datetimespin || sptype == timespin)
	{
		ReturnReactingDateTimeEdit* de = qobject_cast<ReturnReactingDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			return de->dateTime();
	}
	return QDateTime();
}

void BigButtonsSpinbox::setDateTime(const QDateTime& dt)
{
	if (sptype == datespin || sptype == datetimespin || sptype == timespin)
	{
		ReturnReactingDateTimeEdit* de = qobject_cast<ReturnReactingDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			de->setDateTime(dt);
	}
}


bool BigButtonsSpinbox::hasFocus() const
{
	return coreSpinbox->hasFocus();
}

void BigButtonsSpinbox::setInfo(QString& str)
{
	infoLabel->setText(str);
}

void BigButtonsSpinbox::selectAll()
{
	if (sptype == datespin || sptype == datetimespin)
	{
		ReturnReactingDateTimeEdit* de = qobject_cast<ReturnReactingDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
			de->setSelectedSection(ReturnReactingDateTimeEdit::DaySection);
		return;
	}
	coreSpinbox->selectAll();
}

void BigButtonsSpinbox::setStyleSheet(const QString& st)
{
	coreSpinbox->setStyleSheet(st);
}


void BigButtonsSpinbox::timeValueChanged(const QTime& t)
{
	emit timeChanged(t);
    emit valueChanged(coreSpinbox->text());
}

void BigButtonsSpinbox::doubleValueChanged(double v)
{
	emit dvalueChanged(v);
	emit valueChanged(coreSpinbox->text());
}

void BigButtonsSpinbox::editingDone()
{
	if (sptype == datespin || sptype == datetimespin)
	{
		ReturnReactingDateTimeEdit* de = qobject_cast<ReturnReactingDateTimeEdit*>(coreSpinbox);
		if (de != Q_NULLPTR)
		{
			if (de->calendarWidget()->hasFocus())
				return;
		}
		
	}
	if (lastEmit.msecsTo(QTime::currentTime()) > 50)
	{
		emit returnPressed();
		lastEmit = QTime::currentTime();
	}
}

void BigButtonsSpinbox::backRequire()
{
	emit backRequired();
}

void BigButtonsSpinbox::setFocus() const
{
	coreSpinbox->setFocus();
}

void BigButtonsSpinbox::intValueChanged(int t)
{
	emit ivalueChanged(t);
	emit valueChanged(coreSpinbox->text());
}

void ReturnReactingSpinBox::keyReleaseEvent(QKeyEvent* kev)
{
	if (kev->key() == Qt::Key_Return)
		emit returnPressed();
	else
		QSpinBox::keyReleaseEvent(kev);
}

ReturnReactingSpinBox::ReturnReactingSpinBox(QWidget* parent)
	: QSpinBox(parent)
{
}

void ReturnReactingDoubleSpinBox::keyReleaseEvent(QKeyEvent* kev)
{
	if (kev->key() == Qt::Key_Return)
		emit returnPressed();
	else
		QDoubleSpinBox::keyReleaseEvent(kev);
}

ReturnReactingDoubleSpinBox::ReturnReactingDoubleSpinBox(QWidget* parent)
	: QDoubleSpinBox(parent)
{
}

void ReturnReactingDateEdit::keyReleaseEvent(QKeyEvent* kev)
{
	if (kev->key() == Qt::Key_Return)
		emit returnPressed();
	else
		QDateEdit::keyReleaseEvent(kev);
}

ReturnReactingDateEdit::ReturnReactingDateEdit(QWidget* parent)
	: QDateEdit(parent)
{
}

void ReturnReactingDateTimeEdit::keyReleaseEvent(QKeyEvent* kev)
{
	if (kev->key() == Qt::Key_Return)
		emit returnPressed();
	else
		QDateTimeEdit::keyReleaseEvent(kev);
}

ReturnReactingDateTimeEdit::ReturnReactingDateTimeEdit(QWidget* parent)
	:QDateTimeEdit(parent)
{
}

void ReturnReactingTimeEdit::keyReleaseEvent(QKeyEvent* kev)
{
	if (kev->key() == Qt::Key_Return)
		emit returnPressed();
	else
		QTimeEdit::keyReleaseEvent(kev);
}

ReturnReactingTimeEdit::ReturnReactingTimeEdit(QWidget* parent)
	: QTimeEdit(parent)
{
}
