#pragma once
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>



#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"


class SendSettings : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QLabel* aboutWidget;
	QLabel* sendingProtocolInfo;
	QComboBox* protocolPicker;
	QLabel* sendingFormatInfo;
	QComboBox* sendingFormatPicker;
	MegaIconButton* simpleSendingButton;
	MegaIconButton* backButton;

public:
	SendSettings( QWidget* parent);
	virtual bool giveSettings() override;
	virtual void show() override;
private slots:
	void formatChanged(int);
	void protocolChanged(int);
	void simpleSendingToggled();
};