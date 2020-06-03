#include "ScaningSettings.h"
#include <QKeyEvent>
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/BarcodeObserver.h"
#include "widgets/utils/GlobalAppSettings.h"



void Capturer::keyReleaseEvent(QKeyEvent* kev)
{
	emit keyCaptured(kev->key());
	setText(QString::number(kev->key()));
	return;
}

Capturer::Capturer(QWidget* parent)
	: QLineEdit(parent)
{

}

ScaningSettings::ScaningSettings(QWidget* parent)
	: QWidget(parent), mainLayout(new QFormLayout(this)),
	prefixCapturer(new QSpinBox(this)), 
	suffixCapturer(new QSpinBox(this)),
	scanButtonCapturer(new Capturer(this)),
	additionInputElements(new MegaIconButton(this)),
	navigationElements(new MegaIconButton(this)),
	historyButton(new MegaIconButton(this))
{
	mainLayout->addRow(tr("Enter prefix code"), prefixCapturer);
	mainLayout->addRow(tr("Enter suffix code"), suffixCapturer);
	mainLayout->addRow(tr("Scan key setter"), scanButtonCapturer);
	mainLayout->addRow(tr("More input"), additionInputElements);
	mainLayout->addRow(tr("Navigation elements"), navigationElements);
	mainLayout->addRow(tr("Scan history"), historyButton);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	this->setFont(AppGenFont);
	scanButtonCapturer->setText(QString::number(AppSettings->scanButtonCode));
	suffixCapturer->setValue(AppSettings->scanSuffix);
	prefixCapturer->setValue(AppSettings->scanPrefix);
	additionInputElements->setCheckable(true);
	additionInputElements->setChecked(AppSettings->additionalControlElements);
	additionInputElements->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	additionInputElements->setIcon(QIcon(":/res/toinput.png"));

	navigationElements->setCheckable(true);
	navigationElements->setChecked(AppSettings->navigationElements);
	navigationElements->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	navigationElements->setIcon(QIcon(":/res/forward.png"));

	historyButton->setCheckable(true);
	historyButton->setChecked(AppSettings->showHistory);
	historyButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	historyButton->setIcon(QIcon(":/res/pen2.png"));


#ifdef QT_VERSION5X
	QObject::connect(scanButtonCapturer, &Capturer::keyCaptured, this, &ScaningSettings::scanKeyPressed);
#else
	QObject::connect(scanButtonCapturer, SIGNAL(keyCaptured(int)), this, SLOT(scanKeyPressed(int)));
#endif
}

void ScaningSettings::extractAndSave()
{
	AppSettings->additionalControlElements = additionInputElements->isChecked();
	AppSettings->navigationElements = navigationElements->isChecked();
	AppSettings->scanPrefix = prefixCapturer->value();
	AppSettings->scanSuffix = suffixCapturer->value();
	AppSettings->showHistory = historyButton->isChecked();
	BarcodeObs->resetCapture(AppSettings->scanPrefix, AppSettings->scanSuffix, AppSettings->scanButtonCode);
}

void ScaningSettings::scanKeyPressed(int keycode)
{
	AppSettings->scanButtonCode = keycode;
	BarcodeObs->resetCapture(AppSettings->scanPrefix, AppSettings->scanSuffix, AppSettings->scanButtonCode);
}

