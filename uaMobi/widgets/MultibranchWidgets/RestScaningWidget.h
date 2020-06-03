#pragma once
#include "UpdateableScaningWidget.h"


class RestScaningWidget : public UpdateableScaningWidget
{
	Q_OBJECT
protected:
	QString itemCode;
	ClickableTLCounterLabel* restLabel;
	QString requestTemplate;
	virtual void _emplaceBarcode(QString barcode, ShortBarcode info) override;
	virtual void operateOverResponse(QStringList parsedResponse) override;
public:
	RestScaningWidget(Modes mode, QWidget* parent);
protected slots:;
	void sendRestRequest();
};