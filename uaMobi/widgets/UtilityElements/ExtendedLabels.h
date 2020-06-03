#pragma once
#include <QLabel>
#include <cmath>




class TwoLevelCounterLabel : public QLabel
{
	Q_OBJECT
protected:
	QString text;
	double value;
	QString textValue;
	QFont textFont;
	QFont counterFont;
	virtual void paintEvent(QPaintEvent*)override;
public:
    TwoLevelCounterLabel(QString explanation = QString(), double value = std::nan(""), QWidget* parent = Q_NULLPTR);
	void setFonts(double txtpercent, double counterpercent);

public slots:
	void setText(const QString&);
	void setValue(const double& val);
	void clearValue();

};
class ClickableTLCounterLabel : public TwoLevelCounterLabel
{
	Q_OBJECT
protected:
	virtual void mouseReleaseEvent(QMouseEvent*) override;
	virtual void paintEvent(QPaintEvent*)override;
public:
	ClickableTLCounterLabel(QString explanation = QString(), double value = 0.0, QWidget* parent = Q_NULLPTR);
signals:
	void clicked(bool left = true);
};
class ClickableLabel : public QLabel
{
	Q_OBJECT
protected:
	virtual void mouseReleaseEvent(QMouseEvent* mev) override { emit clicked(); QLabel::mouseReleaseEvent(mev); }
public:
	ClickableLabel(QWidget* parent = Q_NULLPTR) :QLabel(parent) {}
signals:
	void clicked();
};

class CounterLabel : public QLabel
{
	Q_OBJECT
protected:
	QString counter;
	void paintEvent(QPaintEvent*);
public:
	CounterLabel(QWidget* parent);
	void setCounter(int cnt);
};


class SemaphorLabel : public QLabel
{
	Q_OBJECT
public:
	enum SemaStates {
		passive,
		awaiting,
		opsuccess,
		opfail
	};
protected:
	int currentState;

	static QColor statedColors[opfail + 1];
	void _recolor();
public:
	SemaphorLabel(int defaultState = passive, QWidget* parent = Q_NULLPTR);
	SemaphorLabel(QWidget* parent = Q_NULLPTR);

public slots:
	void setState(int newState);
};
