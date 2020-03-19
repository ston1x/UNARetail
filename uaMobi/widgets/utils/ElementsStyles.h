#pragma once
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget.h>
#include <QtGui/QScreen>
#include <QString>

#define AppGenFont *FontAdapter::general()
#define AppFonts FontAdapter::instanse()
#define GEOMETRY_SOURCE qApp->screens().first()
/*
	This file contains constants which are defining buttons stylesheets as a C-strings. all
	new styles must be added ONLY here to allow quick change if necessary.
	Naming: \state\_\element\_STYLESHEET

	Update:
		Now this file also stores additional inline functions for calculating adaptive sizes
		of interface elements. Their defaults are usually correspond most used values.
	Update:
		now theese functions are using actual screen size API instead of deprecated desktop()
*/

//These functions are used to calculate size in percent of all window

inline static int calculateAdaptiveButtonHeight(double percent = 0.125)
//Calculates height for buttons, by default is giving 12.5% of screen height
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	percent /= 2;
#endif
#endif
	return qApp->screens().first()->availableGeometry().height() * percent;
}

inline static int calculateAdaptiveWidth(double percent = 0.5)
//Calculates width, by default is giving 50% of screen width
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	percent /= 4;
#endif
#endif
	return qApp->screens().first()->availableGeometry().width() * percent;
}
inline static QSize calculateAdaptiveSize(double percent = 0.3)
//Calculates square size, by default is giving 30% of screen dimensions
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	percent /= 2;
#endif
#endif
	return QSize(
		qApp->screens().first()->availableGeometry().width() * percent,
		qApp->screens().first()->availableGeometry().height() * percent
	);
}
inline static QSize calculateAdaptiveSize(double Hpercent, double Wpercent)
//Calculates more adaptive size, allowing to scale both dimensions. No defaults.
{
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	Hpercent /= 2;
	Wpercent /= 4;
#endif
#endif
	return QSize(
		qApp->screens().first()->availableGeometry().width() * Wpercent,
		qApp->screens().first()->availableGeometry().height() * Hpercent
	);
}

inline QSize imitatePhoneSize(double HPercent)
// sets window size in vertical projection using screen height
{
	return QSize(
		int(GEOMETRY_SOURCE->availableGeometry().height() * (HPercent * 0.66)),
		int(GEOMETRY_SOURCE->availableGeometry().height() * HPercent)
	);
}


class FontAdapter
	// creates scaled fonts
{
	int minheight;		//	minimum height of letter
	int maxheight;		//	maximum height of letter
	double minimumFontPercent;	// default font percent, which used to create original scaling
	int averageCharPerWidth;
	static FontAdapter* _instanse;
	static QFont* _generalFont;

	void _setCharPerWidth();
public:
	FontAdapter(int minheight, int maxheight, double minimumFontPercent);
	void reset(int mh, int Mh, double mfp);
	int howMuchCharacterFitsIntoScreen();
	int getMinFontHeight();
	int getMaxFontHeight();
	double getFontPercent();

	static FontAdapter* instanse();
	static const QFont* general();
	static const QFont makeIndependentFont(int minheight, int maxheight, double minimumFontPercent);
	// creates scaled font by using adapter. Extra percents are relational to calculated minheight
	static QFont makeFont(double extrapercents);
};

extern const QString OK_BUTTONS_STYLESHEET;
// All buttons which are made for confirmation (commit button is separated from simple confirm)

extern const QString BACK_BUTTONS_STYLESHEET;
// Back buttons must be same-styled through all application

extern const QString COMMIT_BUTTONS_STYLESHEET;
// Commit is more saturated

extern const QString DELETE_BUTTONS_STYLESHEET;
// Delete buttons must have this style and provide a warning messagebox

extern const QString CHANGE_BUTTONS_STYLESHEET;
// All buttons which are allowing to change barcode must be same-styled so the user always knew it's functuon by color

extern const QString CHECKED_BUTTONS_STYLESHEET;
// This style is specified to checked state of toggled buttons. use it for settings flags

extern const QString UP_SPINBOX_STYLESHEET;
// up buttons of BigButtonsSpinbox

extern const QString DOWN_SPINBOX_STYLESHEET;
// down buttons of BigButtonsSpinbox

extern const QString LARGE_BUTTON_STYLESHEET;
// Larger buttons

extern QString BETTER_CALENDAR_STYLESHEET();
// Enhanced calendar

extern const QString COUNTERS_LABEL_STYLESHEET;
// bold big letters and borders

extern const QString DOWNLOAD_BUTTONS_STYLESHEET;

extern const QString ENABLED_COUNTERS_STYLESHEET;
extern const QString FOCUSED_DATETIMEEDIT_STYLESHEET;
extern const QString FOCUSED_SPINBOX_STYLESHEET;