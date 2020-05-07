#include "QuickSendSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"


QuickSendSettings::QuickSendSettings(QWidget* parent)
	: QWidget(parent), mainLayout(new QFormLayout(this)),
	aboutWidget(new QLabel(this)),
	protocolPicker(new QComboBox(this)),
	sendingFormatPicker(new QComboBox(this)),
	simpleSendingButton(new MegaIconButton(this))
{
	mainLayout->addRow("", aboutWidget);
	mainLayout->addRow(tr("select protocol"), protocolPicker);
	mainLayout->addRow(tr("select format"), sendingFormatPicker);
	mainLayout->addWidget(simpleSendingButton);

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	aboutWidget->setText(tr("send_settings_about"));
	aboutWidget->setAlignment(Qt::AlignCenter);
    QStringList t;
    t << "Http" << "Local" ;
    protocolPicker->addItems(t);
    t.clear();
	protocolPicker->setCurrentIndex((AppSettings->sendingDirection >= 0 && AppSettings->sendingDirection < 2) ? AppSettings->sendingDirection : 0);
	protocolPicker->setEditable(false);
    t <<  "Xml"<< "txt" << "csv"
       #ifdef QT_VERSION5X
       << "Json"
      #endif
          ;
    sendingFormatPicker->addItems(t);
    t.clear();
	sendingFormatPicker->setCurrentIndex((AppSettings->sendingFormat >= 0 && AppSettings->sendingFormat < 4) ? AppSettings->sendingFormat : 0);
	sendingFormatPicker->setEditable(false);

	simpleSendingButton->setIcon(QIcon(":/res/icn-cloud-blocked-512.png"));
	simpleSendingButton->setText(tr("send_settings_simple_sending"));
	simpleSendingButton->setCheckable(true);
	simpleSendingButton->setChecked(AppSettings->simpleSending);
	simpleSendingButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
}

void QuickSendSettings::extractAndSave()
{
	AppSettings->simpleSending = simpleSendingButton->isChecked();
	AppSettings->sendingDirection = protocolPicker->currentIndex();
	AppSettings->sendingFormat = sendingFormatPicker->currentIndex();
}
