#pragma once
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qprogressbar.h>
#include "externalCommunication/tohttp.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/utils/MegaIconButton.h"
#include <QtCore/qtimer.h>
#include "widgets/SettingsWidgets/GeneralSettings.h"
#include "widgets/SettingsWidgets/SearchDatabaseSettings.h"
#include "widgets/SettingsWidgets/ViewSettings.h"
#include "widgets/SettingsWidgets/DatabaseLoadSettings.h"
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
	DatabaseLoadSettings* dataloadSettings;
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
