#pragma once
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include "widgets/utils/MegaIconButton.h"



class QuickSendSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLabel* aboutWidget;
	QComboBox* protocolPicker;
	QComboBox* sendingFormatPicker;
	MegaIconButton* simpleSendingButton;
public:
	QuickSendSettings(QWidget * parent);
	void extractAndSave();
};