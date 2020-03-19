#pragma once
#include <QWidget>
#include <QBoxLayout>
#include "widgets/utils/MegaIconButton.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <qscrollbar.h>


class ViewSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLabel* example;
	QSpinBox* fontMin;
	QSpinBox* fontMax;
	QSpinBox* fontPercent;

public:
	ViewSettings(QWidget* parent);
	void extractAndSave();
	void retranslate();
protected slots:
	void refreshExample();
};