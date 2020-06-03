#pragma once
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include "widgets/utils/MegaIconButton.h"
#include "dataproviders/ModesDescriptions.h"


class QuickSendSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLabel* aboutWidget;
	QComboBox* protocolPicker;
	QComboBox* sendingFormatPicker;
	MegaIconButton* simpleSendingButton;
	MegaIconButton* attachLastDoc;
	MegaIconButton* cleanLastDoc;
	QLineEdit* docField;
	Modes currentMode;
public:
	QuickSendSettings(Modes mode, QWidget * parent);
	void extractAndSave();
	void attachClicked();
};