#include "GeneralSettings.h"
#include "widgets/utils/GlobalAppSettings.h"
#include <QApplication>

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
GeneralSettings::GeneralSettings(QWidget* parent)
	: QWidget(parent), mainLayout(new QFormLayout(this)), localPath(new QLineEdit(this)),
	langButton(new MegaIconButton(this)),versionControl(new QLabel(this)),
	currlang(indFromLangStr(AppSettings->language))
{
	mainLayout->setLabelAlignment(Qt::AlignCenter);
	mainLayout->setFormAlignment(Qt::AlignCenter);
	mainLayout->addRow(tr("App version"), versionControl);
	mainLayout->addRow(tr("Directory for export"), localPath);
	mainLayout->addRow(tr("App language"), langButton);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	mainLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);
	langicons.push_back(QIcon(filepaths[0]));
	langicons.push_back(QIcon(filepaths[1]));
	langicons.push_back(QIcon(filepaths[2]));
	langButton->setIcon(QIcon(filepaths[indFromLangStr(AppSettings->language)]));
	
	localPath->setText(AppSettings->localfile);
	versionControl->setAlignment(Qt::AlignCenter);
	versionControl->setText(QString::number(VERSION) + SUFFIX);
#ifdef QT_VERSION5X
	QObject::connect(langButton, &MegaIconButton::clicked, this, &GeneralSettings::langChangePressed);
#else
	QObject::connect(langButton, SIGNAL(clicked()), this, SLOT(langChangePressed()));
#endif
}

void GeneralSettings::extractAndSave()
{
	AppSettings->localfile = localPath->text();
}

void GeneralSettings::langChangePressed()
{
	currlang = (currlang + 1 > 2) ? 0 : currlang + 1;
	AppSettings->language = langnames[currlang];
	qApp->removeTranslator(&(AppSettings->qt_translator));
	AppSettings->SetTranslator();
	qApp->installTranslator(&(AppSettings->qt_translator));
	langButton->setIcon(langicons[currlang]);
	emit retranslated();
	qobject_cast<QLabel*>(mainLayout->labelForField(versionControl))->setText(tr("App version"));
	qobject_cast<QLabel*>(mainLayout->labelForField(localPath))->setText(tr("Directory for export"));
	qobject_cast<QLabel*>(mainLayout->labelForField(langButton))->setText(tr("App language"));
}
