#pragma once
#include <QtWidgets/qgridlayout.h>
#include "widgets/utils/MegaIconButton.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
/*
		This widget is the main page widget. it is not a root element, it only
		allows to choose what branch will be opened. Check CoreWidget if you need root widget.
		widgetSelected - returns an enum element mainPageWidgets.
		Update:
			Cleared renundand headers
		Update:
			Now this widget uses isExpectingControls interface of inframeds. Sent number
			is transformed into enum element for signal to be emitted.
*/

namespace mpw {
	enum class mainPageWidgets { Inventory, Supplies, Search, Simple, Camera, Prices };
}
class MainPageWidget : public inframedWidget
{
	Q_OBJECT
private:
	QGridLayout* mainLayout;
	MegaIconButton* inventory;
	MegaIconButton* supplies;
	MegaIconButton* search;
	MegaIconButton* simple;
	MegaIconButton* camera;
	MegaIconButton* prices;
	QHBoxLayout* controlPanel;			//	This overlay is always enabled on any widget
	IgnorantButton* exitButton;			//	quits app
	MegaIconButton* settingsButton;		//	opens settings screen of current child branch
	
	void changeEvent(QEvent* ev) override;
public:
	MainPageWidget(QWidget* parent);
	bool back() override;
	bool giveSettings() override;
	bool isExpectingControl(int contr) override;
public slots:
	void inventoryPressed();
	void suppliesPressed();
	void searchPressed();
	void simplePressed();
	void cameraPressed();
	void pricesPressed();
signals:
	void widgetSelected(mpw::mainPageWidgets);
};
