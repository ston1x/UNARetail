#pragma once
#include <QWidget>
#include <QBoxLayout>
#include "widgets/utils/MegaIconButton.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>


class Capturer : public QLineEdit
{
	Q_OBJECT
protected:
	virtual void keyReleaseEvent(QKeyEvent* kev) override;
public:
	Capturer(QWidget* parent);
signals:
	void keyCaptured(int keycode);
};


class ScaningSettings : public QWidget
{
	Q_OBJECT
protected:
	QFormLayout* mainLayout;
	QLabel* prefixSymbol;
	QSpinBox* prefixCapturer;
	QLabel* suffixSymbol;
	QSpinBox* suffixCapturer;
	Capturer* scanButtonCapturer;
	MegaIconButton* additionInputElements;
	MegaIconButton* navigationElements;
public:
	ScaningSettings(QWidget* parent);
	void extractAndSave();
protected slots:
	void scanKeyPressed(int keycode);
	void updateSymbols(int);
};