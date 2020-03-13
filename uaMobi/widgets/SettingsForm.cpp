#include "SettingsForm.h"

#ifdef DEBUG
#include "debugtrace.h"

debugtrace downloadDebug("downloadlog.txt", detr_supply::all, detr_supply::file);
#endif

#include <QtWidgets/QMessageBox>
#include "widgets/utils/ElementsStyles.h"

static QString filepaths[3] = { ":/res/ro.png", ":/res/ru.png", ":/res/eng.png" };

static QString langnames[3] = { "Romanian", "Russian", "English" };
static int indFromLangStr(const QString& lng)
{
	if (lng == "Russian")
	{
		return 1;
	}
	else if (lng == "Romanian")
	{
		return 0;
	}
	return 2;
}


void SettingsForm::changeEvent(QEvent* pe)
{
	if (pe->type() == QEvent::LanguageChange)
	{
		downloadHttpInfo->setText(tr("settings_form_Http downloads Url"));
		uploadHttpInfo->setText(tr("settings_form_Http uploads Url"));
		localPathInfo->setText(tr("settings_form_Local folder"));
		downloadProgressInfo->setText(tr("settings_form_Download progress"));
		additionalInputSwitch->setText(tr("settings_form_Additional Control Elements"));
		downloadNow->setText(tr("setting_form_Download"));
	}
}

SettingsForm::SettingsForm( QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	downloadHttpInfo(new QLabel(this)),
	uploadHttpInfo(new QLabel(this)), localPathInfo(new QLabel(this)),
	downloadProgressInfo(new QLabel(this)),
	httpDownloadUrl(new QLineEdit(this)),
	httpUploadUrl(new QLineEdit(this)), downloadProgress(new QProgressBar(this)),
	localPath(new QLineEdit(this)),
	additionalInputSwitch(new QPushButton(this)), downloadNow(new QPushButton(this)),
	langButton(new MegaIconButton(this)), backButton(new MegaIconButton(this)),
	keyFilter(new filters::CaptureBackFilter(this)), versionControl(new QLabel("Uamobi Offline v" + QString::number(VERSION) + SUFFIX)),
	actPressing(false), checked("res/single.png"), unchecked("res/data.png"),
	downloadcenter(Modes::Search),
	currlang(indFromLangStr(AppSettings->language)), langicons(), timeoutTimer(new QTimer(this))
{
	mainLayout->addWidget(downloadHttpInfo);
	mainLayout->addWidget(httpDownloadUrl);
	mainLayout->addWidget(uploadHttpInfo);
	mainLayout->addWidget(httpUploadUrl);
	mainLayout->addWidget(localPathInfo);
	mainLayout->addWidget(localPath);
	mainLayout->addWidget(downloadProgressInfo);
	mainLayout->addWidget(downloadProgress);
	mainLayout->addWidget(downloadNow);
	mainLayout->addWidget(additionalInputSwitch);
	mainLayout->addWidget(langButton);
	mainLayout->addStretch();		//	Stretch added to raise main widgets above control panel
	mainLayout->addWidget(versionControl);
	mainLayout->addWidget(backButton);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	downloadHttpInfo->setPixmap(QPixmap(":/res/url.png"));
	downloadHttpInfo->setText(tr("settings_form_Http downloads Url"));
	downloadHttpInfo->setAlignment(Qt::AlignCenter);

	uploadHttpInfo->setPixmap(QPixmap(":/res/url.png"));
	uploadHttpInfo->setText(tr("settings_form_Http uploads Url"));
	uploadHttpInfo->setAlignment(Qt::AlignCenter);

	localPathInfo->setPixmap(QPixmap(":/res/url.png"));
	localPathInfo->setText(tr("settings_form_Local folder"));
	localPathInfo->setAlignment(Qt::AlignCenter);

	downloadProgressInfo->setPixmap(QPixmap(":/res/dowload.png"));
	downloadProgressInfo->setText(tr("settings_form_Download progress"));
	downloadProgressInfo->setAlignment(Qt::AlignCenter);

	versionControl->setAlignment(Qt::AlignCenter);

	additionalInputSwitch->setText(tr("settings_form_Additional Control Elements"));
	additionalInputSwitch->setCheckable(true);
	additionalInputSwitch->setIcon(unchecked);
	additionalInputSwitch->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	additionalInputSwitch->setMinimumHeight(calculateAdaptiveButtonHeight());

	langButton->setIcon(QIcon(filepaths[indFromLangStr(AppSettings->language)]));

	downloadNow->setText(tr("Download"));
	downloadNow->setIcon(QIcon(":/res/netDownload.png"));
	downloadNow->setStyleSheet(DOWNLOAD_BUTTONS_STYLESHEET);
	downloadNow->setMinimumHeight(calculateAdaptiveButtonHeight());

	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	httpUploadUrl->installEventFilter(keyFilter);
	httpDownloadUrl->installEventFilter(keyFilter);
	localPath->installEventFilter(keyFilter);

	langicons.push_back(QIcon(filepaths[0]));
	langicons.push_back(QIcon(filepaths[1]));
	langicons.push_back(QIcon(filepaths[2]));

	timeoutTimer->setInterval(20000);
	timeoutTimer->setSingleShot(true);
	QObject::connect(httpUploadUrl, &QLineEdit::editingFinished, this, &SettingsForm::UrlChanged);
	QObject::connect(httpDownloadUrl, &QLineEdit::editingFinished, this, &SettingsForm::UrlChanged);
	QObject::connect(localPath, &QLineEdit::editingFinished, this, &SettingsForm::UrlChanged);

	QObject::connect(httpDownloadUrl, &QLineEdit::returnPressed, httpUploadUrl,
		QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(httpUploadUrl, &QLineEdit::returnPressed, localPath,
		QOverload<>::of(&QLineEdit::setFocus));

	QObject::connect(additionalInputSwitch, &QPushButton::toggled, this, &SettingsForm::additionalInputtoggled);
	QObject::connect(downloadNow, &QPushButton::pressed, this, &SettingsForm::downloadDataNow);
	QObject::connect(&downloadcenter, &toHttp::progressLeap, this, &SettingsForm::downloadProcessUpdate);
	QObject::connect(keyFilter, &filters::CaptureBackFilter::backRequired, this, &SettingsForm::backRequired);
	QObject::connect(langButton, &MegaIconButton::clicked, this, &SettingsForm::langChangePressed);
	QObject::connect(timeoutTimer, &QTimer::timeout, this, &SettingsForm::downloadTimeout);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SettingsForm::backRequired);
}

bool SettingsForm::back()
{
	return false;
}

bool SettingsForm::giveSettings()
{
	return false;
}

void SettingsForm::show()
{
	httpDownloadUrl->setText(AppSettings->httpIn.toString());
	httpUploadUrl->setText(AppSettings->httpOut.toString());
	localPath->setText(AppSettings->localfile);

	additionalInputSwitch->setChecked(AppSettings->additionalControlElements);
	inframedWidget::show();
}

void SettingsForm::UrlChanged()
// Once any url is changed - all are updated
{
#ifdef DEBUG
	detrace_SLOTCALL("uploadUrlChanged", "SettingsForm");
#endif

	AppSettings->httpIn = httpDownloadUrl->text();
	AppSettings->httpOut = httpUploadUrl->text();
	AppSettings->localfile = localPath->text();
}
void SettingsForm::additionalInputtoggled()
{
	if (additionalInputSwitch->isChecked())
	{
		additionalInputSwitch->setIcon(checked);
	}
	else
	{
		additionalInputSwitch->setIcon(unchecked);
	}
	AppSettings->additionalControlElements = additionalInputSwitch->isChecked();
}

void SettingsForm::downloadDataNow()
//	Updates an address and launchs pull
{
	if (downloadcenter.isUsed())
		return;
	downloadProgress->setValue(1);
	downloadNow->setDisabled(true);
	timeoutTimer->start();
	downloadcenter.addressChanged(AppSettings->httpIn.toString() + "?c=download", true);
	downloadcenter.get();
}

void SettingsForm::downloadProcessUpdate(int proc)
{
	downloadProgress->setValue(proc);
	timeoutTimer->stop();
	if (proc == 100)
	{
		downloadNow->setDisabled(false);
	}
}

void SettingsForm::langChangePressed()
{
	currlang = (currlang + 1 > 2) ? 0 : currlang + 1;
	AppSettings->language = langnames[currlang];
	QApplication::instance()->removeTranslator(&(AppSettings->qt_translator));
	AppSettings->SetTranslator();
	QApplication::instance()->installTranslator(&(AppSettings->qt_translator));
	langButton->setIcon(langicons[currlang]);
}

void SettingsForm::downloadTimeout()
{
	downloadcenter.dropAwaiting();
	downloadNow->setDisabled(false);
}
