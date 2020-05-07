#pragma once
#include <QWidget>
#include <QBoxLayout>
#include "widgets/utils/MegaIconButton.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>
#include "dataproviders/sqldataprovider.h"


class ExtraScaningSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	MegaIconButton* floatControl;
	MegaIconButton* allowPrefixScaning;
	QLineEdit* prefixExtrabarcode;
	MegaIconButton* clearScanBuffer;
	Modes currentMode;
public:
	ExtraScaningSettings(Modes mode, QWidget* parent);
	void extractAndSave();
protected slots:
	void prefixScaningPressed(bool check);
};