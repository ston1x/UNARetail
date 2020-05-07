#include <QApplication>
#ifdef CAMERA_SUPPORT
#include <QZXing.h>
#include <QZXingFilter.h>
#endif
#include "widgets/CoreWidget.h"
#include "debugtrace.h"

#include <QProxyStyle>

class WinCEProxyStyle : public QProxyStyle
{

    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        if (metric == QStyle::PM_ButtonMargin)
            return int(baseStyle()->pixelMetric(metric, option, widget)*2.5);
        /*else if (metric == QStyle::PM_LayoutLeftMargin
                          || metric == QStyle::PM_LayoutTopMargin
                          || metric == QStyle::PM_LayoutRightMargin
                          || metric == QStyle::PM_LayoutBottomMargin
                          || metric == QStyle::PM_LayoutHorizontalSpacing
                          || metric == QStyle::PM_LayoutVerticalSpacing)
                return 2;*/
        else
            return baseStyle()->pixelMetric(metric, option, widget);
    }
};


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	debugtrace::init(detr_supply::all, detr_supply::file);
#ifdef CAMERA_SUPPORT
	QZXing::registerQMLTypes();
	qmlRegisterType<QZXingFilter>("QZXing", 2, 3, "QZXingFilter");
#endif
    qRegisterMetaType<Modes>("Modes");
    qRegisterMetaType<EntityList>("EntityList");
    CoreWidget w;
#ifdef Q_OS_WINCE
    w.move(0, 0);
        a.setStyle(new WinCEProxyStyle());
        w.showNormal();
        w.raise();
        w.move(0, -24);
        w.setWindowFlags((w.windowFlags() | Qt::FramelessWindowHint | Qt::CustomizeWindowHint)
            & ~(Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint
           | Qt::WindowCloseButtonHint | Qt::Dialog | Qt::WindowContextHelpButtonHint));
        w.setFixedHeight(calculateAdaptiveButtonHeight(1));
        w.setFixedWidth(calculateAdaptiveWidth(1));
        QTimer::singleShot(1, &w, SLOT(showMaximized()));
#endif
    w.show();
	return a.exec();
}
