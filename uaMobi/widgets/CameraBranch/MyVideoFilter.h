#ifndef MyVideoFilter_H
#define MyVideoFilter_H

#include <QAbstractVideoFilter>

/*
		This file and associated MyVideoFilterRunnable are hack to overwork the bugged
		QCamera on Android. It is directly drawing with openGl frames obtained from camera.

		DO NOT CHANGE WHILE THIS IS WORKING. UNSTABLE.
			   
*/


class MyVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

    Q_PROPERTY(int orientation MEMBER m_Orientation)

public:
    MyVideoFilter(QObject* parent = nullptr);

    QVideoFilterRunnable* createFilterRunnable() Q_DECL_OVERRIDE;

protected:
    int m_Orientation;

};

#endif
