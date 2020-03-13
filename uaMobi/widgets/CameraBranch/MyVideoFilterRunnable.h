#ifndef MyVideoFilterRunnable_H
#define MyVideoFilterRunnable_H

#include <QVideoFilterRunnable>
#include <QVideoFrame>

/*
		This file and associated MyVideoFilter are hack to avoid QCamera\QViewFinder bug on
		android. They are directly working with openGl to draw frames. 
		DO NOT TOUCH WHILE THIS IS WORKING. UNSTABLE.
*/



class MyVideoFilter;

class MyVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    MyVideoFilterRunnable(MyVideoFilter* parent = nullptr);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) Q_DECL_OVERRIDE;

    static QImage QVideoFrameToQImage(QVideoFrame* input);
    static QImage QVideoFrameToQImage_using_Qt_internals(QVideoFrame* input);
    static QImage QVideoFrameToQImage_using_GLTextureHandle(QVideoFrame* input);
    static void drawRedGreenPixels(QImage& image);

protected:
    MyVideoFilter* m_Filter;
    int m_Orientation;
    int m_Flip;

};

#endif
