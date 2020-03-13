#pragma once
#include <QtWidgets/QPushButton>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/QLineEdit>
#include "widgets/parents/inframedWidget.h"

/*

		This class is holding a realization of simple number on-screen keyboard which provides
		input for users without physicall keyboard.

		TODO: setFocus button

		Update:
			Now this class is incorporated in inframed isExpectingControls system.
			method isExpectingControls is overriden and allows to react to physical keyboard.

*/


class VirtualBarcodeKeyboard : public inframedWidget
	//	Keyboard with number buttons
{
	Q_OBJECT
public:
	enum inputphases { Barcode, Value, Flushing };
private:
	QGridLayout* mainLayout;
	QVector<QPushButton*> numberButtons;	//	here are stored pointers to number buttons
	QPushButton* eraseButton;
	QPushButton* clearButton;
	QPushButton* backButton;
	QPushButton* okButton;
	QLineEdit* barcodeLine;

	inputphases phase;

public:
	explicit VirtualBarcodeKeyboard(QWidget* parent = nullptr);

	virtual bool isExpectingControl(int) override;

private slots:
	void numberPressed();
	void okPressed();
	void backPressed();

public slots:
	void setStartingText(QString&, inputphases);

signals:
	void barcodeConfirmed(QString); // Emitted when user presses OK
	void valueConfirmed(QString);
};