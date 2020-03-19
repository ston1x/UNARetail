#include "ElementsStyles.h"
#include "GlobalAppSettings.h"

/*
	This file contains stylesheet definitions
*/

const QString OK_BUTTONS_STYLESHEET("QPushButton { "
	"background-color: #a2deae;"
	"border: 1px solid gray;"
	"}");

const QString BACK_BUTTONS_STYLESHEET("QPushButton { "
	"background-color: #8ebde6;"
	"border: 1px solid gray;"
	"}");

const QString COMMIT_BUTTONS_STYLESHEET("QPushButton { "
	"background-color: #a0e899;"
	"border: 1px solid gray;"
	"}");

const QString DELETE_BUTTONS_STYLESHEET("QPushButton { "
	"background-color: #d16860;"
	"border: 1px solid gray;"
	"}");

const QString CHANGE_BUTTONS_STYLESHEET("QPushButton {"
	"background-color: #dfe687;"
	"border: 1px solid gray"
	";}");

const QString CHECKED_BUTTONS_STYLESHEET("QPushButton:checked {"
	"background-color: #bdbfa3;"
	"border: 2px solid black;"
	"}");

const QString UP_SPINBOX_STYLESHEET("QPushButton {	"
	"border: 1px solid #e3dac3;"
	"border-top-left-radius: 50px;"
	"border-bottom-left-radius: 50px;"
	"background-color: #61ba68;"
	"}");

const QString DOWN_SPINBOX_STYLESHEET("QPushButton {"
	"border: 1px solid #e3dac3;"
	"border-top-right-radius: 50px;"
	"border-bottom-right-radius: 50px;"
	"background-color: #d6875c;"
	"}");

const QString LARGE_BUTTON_STYLESHEET("QDateEdit{	"
	"border-radius: 5px;"
	"padding: 1px 10px 1px 5px;"
	"border: 1px solid black;"
	"min-height: 30px;"
	"}"
	"QDateEdit::drop-down{"
	"subcontrol-origin: padding;"
	"subcontrol-position: top right;"
	"border-left-width: 1px;"
	"width: 55px;"
	"border-left-style: solid;"
	"border-top-right-radius: 3px;"
	"border-bottom-right-radius: 3px;"
	"border-left-color: black;"
	"image: url(:/res/downarrow.png);"
	"}"
	"QDateEdit::down-arrow{"
	"height:30px;"
	"width:30px;"
	"}");

QString BETTER_CALENDAR_STYLESHEET()
{
	return QString() + "QCalendarWidget QToolButton{"
		+ " height:" + QString::number(calculateAdaptiveButtonHeight(0.05)) + "px;"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.8)) + "px;"
		"color: white;"
		"font-size: " + QString::number(calculateAdaptiveWidth(0.05)) + "px;"
		"icon-size: " + QString::number(calculateAdaptiveWidth(0.116)) + "px, " + 
			QString::number(calculateAdaptiveWidth(0.116)) + "px;"
		"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
		"}"
		"QCalendarWidget QMenu{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.9)) + "px;"
		"left: 20px;"
		"color: white;"
		"font-size: " + QString::number(calculateAdaptiveWidth(0.03)) + "px;"
		"background-color: rgb(100, 100, 100);"
		"}"
		"QCalendarWidget QSpinBox{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.8)) + "px;"
		"font-size:" + QString::number(calculateAdaptiveWidth(0.05)) + "px;"
		"color: white;"
		"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
		"selection-background-color: rgb(136, 136, 136);"
		"selection-color: rgb(255, 255, 255);"
		"}"
		"QCalendarWidget QSpinBox::up-button{"
		"subcontrol-origin: border;"
		"subcontrol-position: top right;"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.13)) + "px;"
		"}"
		"QCalendarWidget QSpinBox::down-button{"
		"subcontrol-origin: border;"
		"subcontrol-position: bottom right;"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.116)) + "px;"
		"}"
		"QCalendarWidget QSpinBox::up-arrow{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.116)) + "px;"
		+ " height:" + QString::number(calculateAdaptiveButtonHeight(0.116)) + "px;"
		"}"
		"QCalendarWidget QSpinBox::down-arrow{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.116)) + "px;"
		+ " height:" + QString::number(calculateAdaptiveButtonHeight(0.116)) + "px;"
		"}"
		"QCalendarWidget QWidget{"
		+ " width:" + QString::number(calculateAdaptiveWidth(0.25))+ "px;"
		+ " height:"+ QString::number(calculateAdaptiveButtonHeight(0.2)) + "px;"
		"alternate-background-color: rgb(128, 128, 128);"
		"}"
		"QCalendarWidget QAbstractItemView : enabled"
		"{"
		"font-size:"+ QString::number(calculateAdaptiveWidth(0.05))+ "px;"
		"color: rgb(180, 180, 180);"
		"background-color: black;"
		"selection-background-color: rgb(64, 64, 64);"
		"selection-color: rgb(0, 255, 0);"
		"}"
		"QCalendarWidget QWidget#qt_calendar_navigationbar"
		"{"
		"background-color: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #cccccc, stop: 1 #333333);"
		"}"
		"QCalendarWidget QAbstractItemView : disabled"
		"{"
		"color: rgb(64, 64, 64);"
		"}";
}
const QString COUNTERS_LABEL_STYLESHEET = "QLabel"
"{"
"border: 2px solid black;"
"font-size: 32px;"
"}";
const QString ENABLED_COUNTERS_STYLESHEET = "QLabel"
"{"
"border: 4px solid #b8dedd;"
"font-size: 32px;"
"}";
const QString DOWNLOAD_BUTTONS_STYLESHEET("QPushButton{ "
	"background-color: #a0e899;"
	"border: 1px solid gray;"
	"} "
	"QPushButton:disabled {"
	"background-color:#4a7039;"
	"}");


const QFont& makeFont(double /*perc*/)
{
	return *(FontAdapter::general());
}

void FontAdapter::_setCharPerWidth()
{
	QFontMetrics fm(*general());
	int width = calculateAdaptiveWidth(0.95);
	if (fm.averageCharWidth() == 0 || width == 0)
		averageCharPerWidth = 10;
	averageCharPerWidth = int(width / fm.averageCharWidth());
}

FontAdapter::FontAdapter(int mh, int mah, double mfp)
	: minheight(mh), maxheight(mah), minimumFontPercent(mfp)
{
#ifdef  Q_OS_WIN
	minimumFontPercent = mfp * 0.6;
#endif //  Q_OS_WIN
}
void FontAdapter::reset(int mh, int Mh, double mfp)
{
	minheight = mh;
	maxheight = Mh;
	minimumFontPercent = mfp;
	*_generalFont = QFont(makeFont(1.0));
	_setCharPerWidth();
}
int FontAdapter::howMuchCharacterFitsIntoScreen()
{
	return averageCharPerWidth;
}
int FontAdapter::getMinFontHeight()
{
	return minheight;
}
int FontAdapter::getMaxFontHeight()
{
	return maxheight;
}
double FontAdapter::getFontPercent()
{
	return minimumFontPercent;
}
FontAdapter* FontAdapter::_instanse = Q_NULLPTR;
QFont* FontAdapter::_generalFont = Q_NULLPTR;
FontAdapter* FontAdapter::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new FontAdapter(AppSettings->fontMinHeight, AppSettings->fontMaxHeight,
			AppSettings->fontPercent);
	}
	return _instanse;
}

const QFont* FontAdapter::general()
{
	if (_generalFont == Q_NULLPTR)
	{
		_generalFont = new QFont(FontAdapter::makeFont(1.0));
		FontAdapter::instanse()->_setCharPerWidth();
	}
	return _generalFont;
}

const QFont FontAdapter::makeIndependentFont(int minheight, int maxheight, double minimumFontPercent)
{
	double currentHeight = qApp->screens().first()->availableGeometry().height();
	currentHeight *= minimumFontPercent;
	if (currentHeight < minheight)
		currentHeight = minheight;
	else
		if (currentHeight > maxheight)
			currentHeight = maxheight;
	return QFont("Times new Roman", int(currentHeight));
}

QFont FontAdapter::makeFont(double extrapercents)
{
	double currentHeight = qApp->screens().first()->availableGeometry().height();
	currentHeight *= FontAdapter::instanse()->minimumFontPercent;
	currentHeight *= extrapercents;
	if (currentHeight < _instanse->minheight)
		currentHeight = _instanse->minheight;
	else
		if (currentHeight > _instanse->maxheight)
			currentHeight = _instanse->maxheight;
	return QFont("Times new Roman", int(currentHeight));
}

const QString FOCUSED_DATETIMEEDIT_STYLESHEET(
	QStringLiteral(
		"QAbstractSpinBox:focus"
		" {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; } "
		"QDateEdit::drop-down {"
		"subcontrol-origin: padding;"
		"subcontrol-position: center right;"
		"width: %1 px;"
		"}"
	)
);
const QString FOCUSED_SPINBOX_STYLESHEET(
	QStringLiteral(
		"QAbstractSpinBox:focus"
		" {"
		"background-color: #d7effa;"
		"border: 2px solid #7ec0de; }"
	)
);