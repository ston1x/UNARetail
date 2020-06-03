#pragma once
#include <QWidget>
#include "dataproviders/ModesDescriptions.h"

class QFormLayout;
class MegaIconButton;
class QLineEdit;
class ExtraScaningSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	MegaIconButton* floatControl;
	MegaIconButton* allowPrefixScaning;
	QLineEdit* prefixExtrabarcode;
	MegaIconButton* clearScanBuffer;
	MegaIconButton* autoFillQuantity;
	Modes currentMode;
public:
	ExtraScaningSettings(Modes mode, QWidget* parent);
	void extractAndSave();
protected slots:
	void prefixScaningPressed(bool check);
};