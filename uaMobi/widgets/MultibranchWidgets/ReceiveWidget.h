#pragma once
#include <QtWidgets/qlabel.h>
#include <QtWidgets/QBoxLayout>
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "externalCommunication/tohttp.h"
#include "externalCommunication/tolocalmemory.h"
#include "widgets/utils/MegaIconButton.h"


/*
		This widget is made to be connected to totcp\tohttp connections, but no functional is provided now

		TODO: block TCP button

		Update:
			inserted protocols. They are not fully ready, but at least they already are available
			all buttons now MegaIcon
			minor size fixes applied
			constructor modified - now GlobalAppSettings are stored here
		Update:
			Fixed AppSettings sync - now show() is updating address fields.
		Update:
			Added connections between servers: now networking can be tested
		Update:
			now tohttp button is sending instead of downloading
			All is functional except toTcp
		Update:
			removed toTcp renundant buttons etc. Removed awaiting-block if user does not confirmed sending
*/

class ReceiveWidget : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QHBoxLayout* infoLayout;
	QLabel* totalQuantity;
	QLabel* uniqueBarcodes;
	QLabel* HttpAddress;
	QLabel* LocalAddress;
	MegaIconButton* useHttpButton;
	MegaIconButton* useLocalButton;
	MegaIconButton* backButton;

	toHttp http;
	toLocalMemory tolocal;

	bool awaiting;
	Modes currentMode;
	sendingMode mode;

public:
	ReceiveWidget(Modes mode, QWidget* parent);

	virtual void show();
	virtual bool isExpectingControl(int value) override;
	virtual bool giveSettings();
	void setMode(sendingMode);
	void show_info();
	bool sendBySetup();
private slots:
	void httpChosen();
	void localChosen();
	void requestEnd(bool);
	void addressFail();
signals:
	void sendingSuccess();
};
