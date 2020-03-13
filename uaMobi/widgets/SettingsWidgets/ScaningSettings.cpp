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
	prefixSymbol(new QLabel(this)),
	prefixCapturer(new QSpinBox(this)), 
	suffixSymbol(new QLabel(this)),
	suffixCapturer(new QSpinBox(this)),
	scanButtonCapturer(new Capturer(this)),
	additionInputElements(new MegaIconButton(this))
{
	mainLayout->addRow(tr("Prefix symbol"), prefixSymbol);
	mainLayout->addRow(tr("Enter prefix code"), prefixCapturer);
	mainLayout->addRow(tr("Suffix symbol"), suffixSymbol);
	mainLayout->addRow(tr("Enter suffix code"), suffixCapturer);
	mainLayout->addRow(tr("Scan key setter"), scanButtonCapturer);
	mainLayout->addRow(tr("More input"), additionInputElements);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	this->setFont(AppGenFont);
	prefixSymbol->setText(QChar(AppSettings->scanPrefix));
	suffixSymbol->setText(QChar(AppSettings->scanSuffix));
	scanButtonCapturer->setText(QString::number(AppSettings->scanButtonCode));
	suffixCapturer->setValue(AppSettings->scanSuffix);
	prefixCapturer->setValue(AppSettings->scanPrefix);
	additionInputElements->setCheckable(true);
	additionInputElements->setChecked(AppSettings->additionalControlElements);
	additionInputElements->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	additionInputElements->setIcon(QIcon(":/res/toinput.png"));

	QObject::connect(suffixCapturer, QOverload<int>::of(&QSpinBox::valueChanged),
		this, &ScaningSettings::updateSymbols);
	QObject::connect(prefixCapturer, QOverload<int>::of(&QSpinBox::valueChanged),
		this, &ScaningSettings::updateSymbols);
	QObject::connect(scanButtonCapturer, &Capturer::keyCaptured, this, &ScaningSettings::scanKeyPressed);
	
}

void ScaningSettings::extractAndSave()
{
}

void ScaningSettings::scanKeyPressed(int keycode)
{
	AppSettings->scanButtonCode = keycode;
}

void ScaningSettings::updateSymbols(int)
{
	suffixSymbol->setText(QChar(suffixCapturer->value()));
	prefixSymbol->setText(QChar(prefixCapturer->value()));
}
