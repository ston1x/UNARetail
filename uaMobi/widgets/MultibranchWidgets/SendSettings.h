#pragma once
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>


#include "widgets/utils/MegaIconButton.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/SettingsWidgets/QuickSendSettings.h"
#include "widgets/SettingsWidgets/SerializationSettings.h"


class SendSettings : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QTabWidget* innerWidget;
	QuickSendSettings* quickSendSettings;
	SerializationSettings* serializationSettings;
	MegaIconButton* backButton;



public:
	SendSettings(Entity prototype, QWidget* parent);
private slots:;
	void saveAndExit();
};