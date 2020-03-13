#pragma once
#include "abs_control.h"
#include <QLabel>

namespace specwidgets
{
	class unfocusableLabel : public QLabel
	{
		Q_OBJECT
	private:
		virtual void keyReleaseEvent(QKeyEvent* kev) override
		{
			if (kev->key() == Qt::Key_Return)
			{
				emit returnPressed();
			}
			else
			{
				QLabel::keyReleaseEvent(kev);
			}
		}
	public:
		explicit unfocusableLabel(QString& rext, QWidget* parent)
            : QLabel(rext, parent) {}
	signals:
		void returnPressed();
	};

}

class LabelControl : public abs_control
{
	Q_OBJECT
private:
	specwidgets::unfocusableLabel* innerLabel;

	// Inherited via abs_control
	virtual QString prepareAndReturnValue() const override;
	virtual bool parseAndSetValue(QString) override;
	virtual bool valueAvailable() const override;
	virtual void clear() override;
	virtual QWidget* getInnerWidget() const override;
public:
	explicit LabelControl(QString str, QWidget* parent);
    virtual ~LabelControl()
#ifdef QT_VERSION5X
    override
#endif
    ;
	virtual void setFocus() const override;
	virtual void show() override;
	virtual void hide() override;

};
