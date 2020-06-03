#include "QuickSendSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include <QLineEdit>

QuickSendSettings::QuickSendSettings(Modes mode, QWidget* parent)
	: QWidget(parent), mainLayout(new QFormLayout(this)),
	aboutWidget(new QLabel(this)),
	protocolPicker(new QComboBox(this)),
	sendingFormatPicker(new QComboBox(this)),
	simpleSendingButton(new MegaIconButton(this)),
	attachLastDoc(new MegaIconButton(this)),
	cleanLastDoc(new MegaIconButton(this)),
	docField(new QLineEdit(this)),
	currentMode(mode)
{
	mainLayout->addRow("", aboutWidget);
	mainLayout->addRow(tr("select protocol"), protocolPicker);
	mainLayout->addRow(tr("select format"), sendingFormatPicker);
	mainLayout->addRow(tr("attach Last Doc"), attachLastDoc);
	mainLayout->addRow(tr("clean doc"), cleanLastDoc);
	mainLayout->addRow(tr("current last document"), docField);
	mainLayout->addWidget(simpleSendingButton);
	mainLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
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
	attachLastDoc->setIcon(QIcon(":/res/add.png"));
	attachLastDoc->setCheckable(true);;
	attachLastDoc->setChecked(AppSettings->getModeDescription(mode).requiresAttachingToPreviousDoc());
	attachLastDoc-> setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	cleanLastDoc->setIcon(QIcon(":/res/refresh.png"));
	cleanLastDoc->setCheckable(true);
	cleanLastDoc->setChecked(AppSettings->getModeDescription(mode).mustClearBeforeAttaching());
	cleanLastDoc->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	docField->setEnabled(attachLastDoc->isChecked());
	cleanLastDoc->setEnabled(attachLastDoc->isChecked());
	
	docField->setText(QString::number(AppSettings->getModeDescription(mode).getPreviousDocNumber()));
	QObject::connect(attachLastDoc, &MegaIconButton::clicked, this, &QuickSendSettings::attachClicked);

}

void QuickSendSettings::extractAndSave()
{
	AppSettings->simpleSending = simpleSendingButton->isChecked();
	AppSettings->sendingDirection = protocolPicker->currentIndex();
	AppSettings->sendingFormat = sendingFormatPicker->currentIndex();
	AppSettings->getModeDescription(currentMode).setAttachingToPrevDoc( attachLastDoc->isChecked());
	QString line = docField->text();
	if (!line.isEmpty())
	{
		bool ok;
		long long int temp = docField->text().toLongLong(&ok);
		if (!ok)
			AppSettings->getModeDescription(currentMode).setPreviousDocument(0);
		else
			AppSettings->getModeDescription(currentMode).setPreviousDocument(temp);
	}
	else
		AppSettings->getModeDescription(currentMode).setPreviousDocument(0);
	AppSettings->getModeDescription(currentMode).setCleanBeforeAttaching(cleanLastDoc->isChecked());

}

void QuickSendSettings::attachClicked()
{
	docField->setEnabled(attachLastDoc->isChecked());
	cleanLastDoc->setEnabled(attachLastDoc->isChecked());
	if (!attachLastDoc->isChecked())
	{
		docField->clear();
	}
	else
		cleanLastDoc->setChecked(AppSettings->getModeDescription(currentMode).mustClearBeforeAttaching());
}
