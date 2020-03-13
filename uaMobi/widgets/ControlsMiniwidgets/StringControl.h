#pragma once
#include "abs_control.h"
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>

class StringControl : public abs_control
{
	Q_OBJECT
private:
	QWidget * innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* about;
	QLineEdit* input;

	// Inherited via abs_control
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;
	virtual QWidget* getInnerWidget() const override;
	
public:
	StringControl(QString name, QWidget* parent);
    virtual ~StringControl()
#ifdef QT_VERSION5X
    override
#endif
    ;
	virtual void setFocus() const override;
	virtual void show() override;
	virtual void hide() override;
};

