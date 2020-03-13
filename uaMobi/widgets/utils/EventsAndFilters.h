#pragma once
#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
/*
	This file contains everything related to handling events. Mostly KeyEvents
		NoKeyEvents:
			Despite it's name it does not eat key releases: it just intercepts them and
			emits corresponding signals. Main goal of this class is to keep outer reactions
			of widget while shutting down it's own. Example of usage: class ScaningWidget.
			There is barcodelist - a ListWidget, which must not react to number key presses,
			but all numbers must be counted. So this filter intercepts them and puts into signals.

		CaptureBackFilter:
			This filter is lighter version of NoKeyEvents. Only reacts to back key.

		GeneralPurposeFilter:
			This filter is multi-option filter which can be adjusted for filtering a specified group of events.
			it uses pointers for enhancing filtering, so it is faster than simple if ladder.
*/

namespace filters
{
	class NoKeyEvents : public QObject	//	intercepts key events to emit own signals
	{
		Q_OBJECT
	protected:

		bool eventFilter(QObject* object, QEvent* ev);

	public:
		NoKeyEvents(QObject* ob) : QObject(ob) {};
	signals:
		void numberObtained(QString);	//	holds number ready to be appended
		void returnObtained();			//	return was pressed
		void backRequired();	//	back or escape was pressed
		void unknownObtained(); //  unknown key code was captured. Usually they belong to device-specific buttons
		void eraseRequired();	//  backspace was pressed
	};

	class CaptureBackFilter : public QObject
	{
		Q_OBJECT
	protected:
		bool eventFilter(QObject* object, QEvent* ev);
	public:
		CaptureBackFilter(QObject* ob) : QObject(ob) {};
	signals:
		void backRequired();
	};
	class LineEditHelper : public QObject
	{
		Q_OBJECT
	protected:
		bool eventFilter(QObject* watched, QEvent* event) override;
	public:
		LineEditHelper(QObject* ob) : QObject(ob) {};
	};
	class GeneralPurposeFilter : public QObject
	{
		Q_OBJECT
	public:
		enum captureTypes { Number, Return, Back, Scan, Symbol, All, infPack };
	protected:

		captureTypes mytypes[5];
		int typeLen;
		typedef	bool(GeneralPurposeFilter::* CaptureFunction)(QKeyEvent*);
		CaptureFunction captureFunction;

		CaptureFunction someCaptureFunctions[5];

		// util fuction that turns int into pointer to capture function
		CaptureFunction makeCaptureFunction(captureTypes type);

		bool eventFilter(QObject* ob, QEvent* ev) override;

		//	These functions are used for capturing and are called during eventFiltering
		//	These one are element captures that search for only one type
		bool captReturn(QKeyEvent*);
		bool captNumber(QKeyEvent*);
		bool captSymbol(QKeyEvent*);
		bool captBack(QKeyEvent*);
		// These are complex capturing fuctions that call element captures
		bool captSome(QKeyEvent*);
		bool captScan(QKeyEvent*);
		bool captInf(QKeyEvent*);
		//This is aggregate-capturing function that is implemented without element-calls to enhance performance
		bool captAll(QKeyEvent*);

	public:
		GeneralPurposeFilter(captureTypes type = All, QObject* parent = Q_NULLPTR);
		GeneralPurposeFilter(captureTypes type1, captureTypes type2, QObject* parent = Q_NULLPTR, captureTypes type3 = All, captureTypes type4 = All, captureTypes type5 = All);
	signals:
		void returnPressed();	//	reflects return press
		void backPressed();		//	reflects back press
		void scanPressed();		//	reflects scan press
		void numberPressed(int);	//	reflects pressed number buttons
		void symbolPressed(QString);	//	reflects any pressed text
		void scanReleased();
	};
}
