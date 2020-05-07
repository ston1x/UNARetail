#pragma once
#include "ScaningWidget.h"



class AsyncRequestEngine;
class UpdateableScaningWidget : public ScaningWidget
{
	Q_OBJECT
protected:
	AsyncRequestEngine* datarefreshEngine;
	virtual void _requestUpdate(QString request);


public:
	UpdateableScaningWidget(Modes mode, QWidget* parent);
protected slots:
	virtual void operateOverPureResponse(QString);
	virtual void operateOverResponse(QStringList parsedResponse) = 0;
	virtual void handleError(int);
};