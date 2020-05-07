#pragma once
#include <chrono>
#include <QtCore/QString>
#include <QtCore/QVector>

/*
	In this file is stored class-controller of input values. Main idea of this class is to provide incapsulation
	to comlicated algorythm of interpreting input. It handles input by dividing it into 3 phases:
		Scaning:		This part of processing is expecting barcode. It refuses any input which is provided too slow. Only one flag
					is raised: isScaning. This phase is checking if previous phase ended succesfully - if not, it refuses to launch again.
					Normal delay between input: 30 ms ( scanner-provided values always are faster  - 0-5 ms. All input will be thrown in
					barcodeBuffer.
		Quantity:		This part is expecting quantity. It assumes that any non-user input is wrong and blocks button presses faster than 30ms.
					It manipulates with large amount of flags:  quantityAwaited, incrementRequired, quantityObtained. Normal behaviour is to
					raise incrementRequired until user will not provide input. User can force-end this phase with pressing Return -> it will mark quantity
					as obtained, but will leave increment required raised - this will allow to increment value later. All input will be filtered: only numbers are
					permitted. Input is thrown into quantityBuffer.
		Flushing:		This part is awaiting while user will extract values, while ignoring any input. It usually raises its own flag.

	Update:
		In this file now stored Multistate Scan Engine, which is different from old scanState - it is prepared from abstract usage for any quantity of values
		possibly it is ok to use it instead of old scanstate.
*/

class scanState
	// This class is used to track input state and separate it into two listening phases and third semi-phase
	// Phase line is Scaning->Quantity->Flushing. Flushing is semi-phase which comes after confirming quantity -> in it all values are wiped
{
protected:
	bool barcodePending;	//	barcode obtained and now stored in buffer
	bool quantityAwaited;	//	next input will be placed in quantity buffer
	bool canGiveBarcode;	//	barcode obtained and confirmed, no additional input will be processed
	bool quantityObtained;	//	quantity buffer contains value which can be used in input
	bool incrementRequired;	//  quantity was empty when quantity-reading phase was interrupted. You must rewrite quantity after extracting it.
	bool isScaning;			//	next input will be placed in quantity buffer
	bool elementsReadyForExtraction;
	QString barcodeBuffer;	//	here is stored input for barcode
	QString quantBuffer;	//	here is stored input for quantity
	std::chrono::high_resolution_clock::time_point lastPress;	//	This time point determines last press event of scan button.
	std::chrono::high_resolution_clock::time_point quantityBufferTrigger;
	std::chrono::high_resolution_clock::time_point barcodeTrigger;
public:
	/*
			Phase flags note (only raised):
				Scaning: isScaning!
				Quantity:	quantityAwaited! canGiveBarcode! incrementRequired! barcodePending!
				Flushing:  canGiveBarcode! incrementRequired || quantityObtained!

	*/

	scanState();		//	constructs empty state with false states. Press scan to start.

	void pressScan();	//	Sets up variables to process barcode input
	void pressReturn();	//	moves to next phase in row : barcode -> quantity -> flushing
	void barcodeElementReleased(QString&);	//	writes element to corresponding buffer
	QString getBarcode();	//	returns barcode and erases buffer, then resets variables connected to barcode
	QString getQuantity();	//	returns quantity and erases buffer, then resets variables connected to quantity
	QString peekQuantity();	//	returns quantity, but leaves buffer and variables untouched. Usefull for displaying non-complete value
	QString peekBarcode();	//	returns barcode, but leaves buffer and variables untouched.
	bool barcodeReadyForExtraction();	//	checks inner variables and returns true if barcode is stored in buffer
	bool requiresIncrement();	//	checks inner variables and returns true if there is no legit quantity value in buffer
	bool chopQuantity();	//	erases last number of quantity
	bool expectQuantity();	//	returns true if current phase is quantity
	void forceSetQuantity(int); // Use this variable to rewrite quantity buffer with outer value
	bool hasQuantity();
	bool canTakeElements();
	void clear();
	bool isExpectingBarcode();
	QString stateOfEngine();	//	returns string which contais current state of all inner variables
};

class MultistateScanEngine
	// This class is used to track abstragized input which must fill some amount of values. It wraps vector of such values and releases them 
	// when all values are filled. All distribution mechanism is covered under standard reactions to pressed scan - pressed return.
{
protected:
	bool isScaning;		//	Indicator of barcode scanning phase
	bool canGiveBarcode;	// indicates that scanning phase passed and barcode is now stored inside

	bool valueAwaited;	//	indicator of value awaiting phase
	bool valuesObtained;	//	indicates that all values are obtained
	bool elementsReadyForExtraction;	//	indicator of Flushing phase

	bool valueRequiresDefault;		//	indicates that current value and all remaining can be filled by default

	int totalValues;			//	maximum values expected
	int currentValueIndex;		//	index of the value which is filled in current phase
	QVector<QString> valuesBuffer;	//	buffer which stores input during Value Awaiting phase
	int lastRequired;			//	last value index which must be awaited without overriding it with default value

	QString barcodeBuffer;		//	holds barcode
	std::chrono::high_resolution_clock::time_point lastPress;	//	Is renundant, but may be usefull
	std::chrono::high_resolution_clock::time_point valuesBuffersTrigger;	//	Is used for tracking intervals between values input. It must be slow
	std::chrono::high_resolution_clock::time_point barcodeTrigger;			//	Is used for tracking intervals between barcode input. It must be fast.

	virtual void scaningPhaseProcessing(QString& elem);	//	submethod for operations over values in scanning phase
	virtual void valuesPhaseProcessing(QString& elem);	//	submethod for operations over values in Value Awaiting phase
	virtual void flushingPhaseProcessing(QString& elem);//	submethod for operations over values in Flushing phase
	void clearValBuffer();								//	correctly erases value buffer
public:

	MultistateScanEngine(int valuesToAwait, int LastRequired = 20000);	//	Creates empty engine. Press scan to start.

	void pressScan();				//	Main interface method - imitates scan button press
	void pressReturn();				//	Main interface method - imitates return button press
	void processNonControlInput(QString&);	//	Main interface method - imitates any keyboard press except scan and return
	QString getBarcode();			//	returns barcode and then erases it - this method usually must be called in Flushing phase
	QString peekCurrentValue();		//	returns value that now is awaited
	QString peekBarcode();			//	returns barcode without erasing it
	QString peekValueAt(int);		//	returns value under provided index
	bool barcodeReadyForExtraction();	//	true if barcode is valid and ready
	bool isAwaitingBarcode();
	bool chopCurrentValue();		// chops current value. False - if there is nothing to chop
	bool isExpectingValue();		// true if value is awaited
	void forceSetValue(int);		//	overwrites current value with another one
	bool hasAllValues();			//	returns true if last value is now in buffer - but this is not indicating Flushing phase
	bool canTakeElements();			//	indicates Flushing phase
	void clear();					//	drops engine's state to zero - same as creating new engine
	int getCurrentValueIndex();		//	returns current index
	bool currentValueRequiresDefault();	//	true if this value can be replaced with default value
	QVector<int> getConvertedValues();	//	returns vector of values which already are prepared for inserting into barcode. Erases buffer.
	QVector<QString> getValues();	//	returns unconverted values. Erases buffer
	QString stateOfEngine();		//	returns debugging string describing current state of engine
};