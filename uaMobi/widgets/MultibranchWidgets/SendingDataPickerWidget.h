#pragma once
#include "widgets/MultibranchWidgets/ReceiveWidget.h"
#include "widgets/MultibranchWidgets/SendSettings.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/UtilityElements/ExtendedLabels.h"
/*
	This class represents widget used to pick which data must be sent. It has a child ReceiveWidget,
	so it represents one subbranch

*/

class SendingDataPickerWidget : public inframedWidget, abstractNode
	//	Subbranch used for sending data
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;		//	view
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	SemaphorLabel* semaphor;
	MegaIconButton* unsentButton;
	MegaIconButton* sentButton;
	MegaIconButton* allButton;
	MegaIconButton* settingsButton;
	CounterLabel* sentQuantityInfo;
	CounterLabel* unsentQuantityInfo;
	CounterLabel* totalQantityInfo;
	MegaIconButton* backButton;

	Modes currentMode;
	
	ReceiveWidget* sendWidget;
	SendSettings* settingsWidget;

public:
	SendingDataPickerWidget( Modes mode,  QWidget* parent);
	virtual void show() override;
	virtual bool back() override;
	virtual bool isExpectingControl(int) override;
    virtual bool giveSettings() override;
protected slots:
	void unsentChosen();
	void sentChosen();
	void allChosen();
	void hideCurrent();
	void sendingSuccess();
	void showSettings();
    void set_info();
signals:
	void sendingSuccesfull();
};
