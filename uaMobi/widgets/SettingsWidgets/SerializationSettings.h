#pragma once
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include "widgets/utils/MegaIconButton.h"
#include <QListWidget>
#include "Datacore/DataEntities.h"
#include "dataproviders/ModesDescriptions.h"

class SerializationSettings : public QWidget
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QListWidget* fieldPicker;

	QStringList fieldsModel;
	Entity prototype;
	Modes currentMode;
public:
	SerializationSettings(Modes mode, Entity prototype, QWidget* parent);
	void extractAndSave();
};