#pragma once
#include <QBoxLayout>
#include <QComboBox>
#include <QLabel>


#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/SettingsWidgets/QuickSendSettings.h"
#include "widgets/SettingsWidgets/SerializationSettings.h"
#include "widgets/SettingsWidgets/ExtraSendSettings.h"
#include "widgets/SettingsWidgets/LoginSettings.h"
class SendSettings : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QTabWidget* innerWidget;
	QuickSendSettings* quickSendSettings;
	SerializationSettings* serializationSettings;
	ExtraSendSettings* extraSettings;
	LoginSettings* loginSettings;
	MegaIconButton* backButton;
	Modes currentMode;


public:
	SendSettings(Modes cmode, Entity prototype, QWidget* parent);
	void showExtraSettings();
private slots:
	void saveAndExit();
};
