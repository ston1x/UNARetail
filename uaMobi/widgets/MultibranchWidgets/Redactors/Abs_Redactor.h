#pragma once
#include <QWidget>
#include <QFormLayout>
#include "widgets/utils/BigButtonsSpinbox.h"
#include "widgets/utils/MegaIconButton.h"
#include <QLabel>
#include "Datacore/DataEntities.h"


class Abs_Redactor : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLabel* barcodeInfo;
	QLabel* additionalInfo;
	
public:
	Abs_Redactor(QWidget* parent);
	virtual bool setRedacted(Entity e) = 0; 
	virtual void extractAndSave() = 0;
signals:
	void done();
};