#pragma once
#include <QWidget>
#include <QBoxLayout>
#include "widgets/utils/MegaIconButton.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <dataproviders/sqldataprovider.h>

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
	QSpinBox* prefixCapturer;
	QSpinBox* suffixCapturer;
	Capturer* scanButtonCapturer;
	MegaIconButton* additionInputElements;
	MegaIconButton* navigationElements;
	MegaIconButton* historyButton;
public:
	ScaningSettings( QWidget* parent);
	void extractAndSave();
protected slots:
	void scanKeyPressed(int keycode);
};