#include "ExtraSendSettings.h"
#include "widgets/utils/GlobalAppSettings.h"


QStringList sysfeedPseudonims;
QVector<int> sysfeeds;


void ExtraSendSettings::_initSysfeeds()
{
	if (!sysfeeds.isEmpty())
		return;
	sysfeedPseudonims <<
		tr("scanAct") <<
		tr("shop-LC") <<
		tr("custom");
	sysfeeds <<
		12095 <<
		1233 <<
		-1;
}

ExtraSendSettings::ExtraSendSettings(Modes cmode, QWidget* parent)
	: QWidget(parent), mainLayout(new QFormLayout(this)),
	sysfeedPicker(new QComboBox(this)), customSysfeedPicker(new QSpinBox(this)),
	currentMode(cmode)
{
	mainLayout->addRow(tr("Push in document"), sysfeedPicker);
	mainLayout->addRow(tr("Custom sysfeed"), customSysfeedPicker);

	customSysfeedPicker->setMinimum(-1);
	customSysfeedPicker->setMaximum(200000);

	_initSysfeeds();
	sysfeedPicker->addItems(sysfeedPseudonims);
	sysfeedPicker->setEditable(false);
	int ok = false;
	for (int i = 0; i < sysfeeds.count(); ++i)
	{
		if (AppSettings->sysfeed.at(int(currentMode)) == sysfeeds.at(i))
		{
			sysfeedPicker->setCurrentIndex(i);
			ok = true;
			break;
		}
	}
	if (ok)
	{
		customSysfeedPicker->setEnabled(false);
	}
	else
	{
		sysfeedPicker->setCurrentIndex(sysfeeds.count() - 1);
		customSysfeedPicker->setValue(AppSettings->sysfeed.at(int(currentMode)));
	}


#ifdef QT_VERSION5X
	QObject::connect(sysfeedPicker, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ExtraSendSettings::itemSelected);
#else
	throw;
#endif
}

void ExtraSendSettings::extractAndSave()
{
	if (sysfeeds.at(sysfeedPicker->currentIndex()) == -1)
	{
		AppSettings->sysfeed[int(currentMode)] = customSysfeedPicker->value();
	}
	else
	{
		AppSettings->sysfeed[int(currentMode)] = sysfeeds.at(sysfeedPicker->currentIndex());
	}
}

void ExtraSendSettings::itemSelected(int index)
{
	if (sysfeeds.at(index) == -1)
	{
		customSysfeedPicker->setEnabled(true);
	}
	else
	{
		customSysfeedPicker->setEnabled(false);
	}
}
