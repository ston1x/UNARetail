#pragma once
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qprogressbar.h>
#include "externalCommunication/tohttp.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/utils/MegaIconButton.h"
#include <QtCore/qtimer.h>
#include "widgets/SettingsWidgets/GeneralSettings.h"
#include "widgets/SettingsWidgets/SearchDatabaseSettings.h"
#include "widgets/SettingsWidgets/ViewSettings.h"
/*
*/

class SettingsForm : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QTabWidget* innerWidget;
	GeneralSettings* generalSettings;
	SearchDatabaseSettings* databaseSettings;
	ViewSettings* viewSettings;
	MegaIconButton* backButton;
	
public:
	SettingsForm( QWidget* parent);

public slots:
	void retranslation();
	void saveAndExit();
signals:
	void retranslated();
	void fontsChanged();
};