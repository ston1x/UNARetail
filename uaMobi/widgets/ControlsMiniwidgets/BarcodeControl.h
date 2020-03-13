#pragma once
#include "abs_control.h"
#include <QLineEdit>
#include "ScaningCore/BarcodeObserver.h"



class BarcodeControl : public abs_control
{
	Q_OBJECT
private:
	QLineEdit* innerLineEdit;
	bool requiresDeactivation;
	// Inherited via abs_control
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual void clear() override;
	virtual bool valueAvailable() const override;
	virtual QWidget* getInnerWidget() const override;
public:
	BarcodeControl( QString name, QWidget * parent );
    virtual ~BarcodeControl()
#ifdef QT_VERSION5X
    override
#endif
    ;
	virtual void setFocus() const override;
	virtual void show() override;
	virtual void hide() override;
private slots:
	void proxyEditingFinished();
};
