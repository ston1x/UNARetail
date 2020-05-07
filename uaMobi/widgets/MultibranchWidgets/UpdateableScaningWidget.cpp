#include "UpdateableScaningWidget.h"
#include "externalCommunication/AsyncRequestEngine.h"
#include "widgets/utils/GlobalAppSettings.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif


void UpdateableScaningWidget::_requestUpdate(QString request)
{
	datarefreshEngine->clearAndRequest(request);
}

UpdateableScaningWidget::UpdateableScaningWidget(Modes mode, QWidget* parent)
	:ScaningWidget(mode, parent), datarefreshEngine(new AsyncRequestEngine(AppSettings->httpIn.toString(), true, this))
{
	QObject::connect(datarefreshEngine, &AsyncRequestEngine::clearResponse, this, &UpdateableScaningWidget::operateOverPureResponse);
	QObject::connect(datarefreshEngine, &AsyncRequestEngine::parsedResponse, this, &UpdateableScaningWidget::operateOverResponse);
	QObject::connect(datarefreshEngine, &AsyncRequestEngine::errorResponse, this, &UpdateableScaningWidget::handleError);
}

void UpdateableScaningWidget::handleError(int errcode)
{
#ifdef DEBUG
	detrace_NETERROR(errcode, "in updateable scaning");
#endif
}

void UpdateableScaningWidget::operateOverPureResponse(QString)
{
	return;
}
