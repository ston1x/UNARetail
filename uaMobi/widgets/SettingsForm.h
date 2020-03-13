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

/*
		This class is providing simple settings form for mainPage. It is default for
		settings button.

		Update: now changes are reflected to GlobalAppSettings

		Update: renamed ActiveTouch into additional interface. Now this is reflected to GlobalAppSettings

		Major update:
			Reworked fields: separated download and upload addresses, added field for local address
			Added networking as toHttp object: downloadNow button launches download
		Update:
			Interaction added - now jumps between lineedits are online. Download is ready.
		Update:
			Removed some renundant fields
*/

class SettingsForm : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QLabel* downloadHttpInfo;
	QLabel* uploadHttpInfo;
	QLabel* localPathInfo;
	QLabel* downloadProgressInfo;
	QLineEdit* httpDownloadUrl;
	QLineEdit* httpUploadUrl;
	QLineEdit* localPath;
	QProgressBar* downloadProgress;
	QPushButton* additionalInputSwitch;
	QPushButton* downloadNow;
	MegaIconButton* langButton;
	MegaIconButton* backButton;
	filters::CaptureBackFilter* keyFilter;
	QLabel* versionControl;

	bool actPressing;

	QIcon checked;
	QIcon unchecked;

	toHttp downloadcenter;
	
	int currlang;

	QVector<QIcon> langicons;

	QTimer* timeoutTimer;

	void changeEvent(QEvent* pe);
public:
	SettingsForm( QWidget* parent);
	bool back() override;
	bool giveSettings() override;
	void show() override;

public slots:
	void UrlChanged();
	void additionalInputtoggled();
	void downloadDataNow();
	void downloadProcessUpdate(int proc);
	void langChangePressed();
	void downloadTimeout();
signals:
	void backPressed();
	void quitRequired();
};