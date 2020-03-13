#pragma once
#include <QtWidgets/qpushbutton.h>

/*
	This class is overriding paintEvent to make icons very big. Usefull for mobile dev.
	Icon will occupy 70% of button square, while text will be placed below.

	Update: refreshed scaling: now it is allowed to change from outside. use setScaleCoefficient

	Fix: fixed zero coefficient in empty constructor

*/

class MegaIconButton : public QPushButton
	//	Button with better icon handling
{
	Q_OBJECT
private:
	QIcon ico;		//	Icon which will be displayed
	double imageCoefficient;	//	Coefficient of image size
protected:
	void paintEvent(QPaintEvent*);	//	Draws button
public:
	explicit MegaIconButton(QWidget* parent = nullptr);
	MegaIconButton(QIcon  ico, const double imageCoef = 0.7, const QString txt = QString(), QWidget* parent = nullptr);
	void setIcon(QIcon ico); // sets icon
	void setScaleCoefficient(double coe);	//	sets scale coefficient. Warning: does not redrawing button immidiately
}
;
class IgnorantButton : public MegaIconButton
{
	Q_OBJECT
protected:
	void keyPressEvent(QKeyEvent*) {};
	void keyReleaseEvent(QKeyEvent*) {};
public:
	explicit IgnorantButton(QWidget* parent = nullptr) : MegaIconButton(parent) {};
};