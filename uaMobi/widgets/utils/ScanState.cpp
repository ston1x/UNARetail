#include "ScanState.h"
#include <QtCore/qtextstream.h>
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

using namespace std::chrono;

scanState::scanState()
// Default constructor is empty. you need to pressScan() to begin from Scaning phase
	: barcodePending(false), quantityAwaited(false), canGiveBarcode(false), quantityObtained(false)
	, incrementRequired(false), isScaning(false), barcodeBuffer(), quantBuffer()
{
}

void scanState::pressScan()
// This method is telling scanState that scan button is pressed. It can be triggered any time
{
	elementsReadyForExtraction = canGiveBarcode && quantityObtained;	//	Refreshes this flag to be sure that it reflects actual state
	if (elementsReadyForExtraction)		//	if they are ready, phase is changed to Flushing and quantity listening is stopped.
	{
		quantityAwaited = false;
		return;	//	elements must be extracted
	}
	if (isScaning)
		return;
	barcodeBuffer.clear();		//	If it is not Flushing - new Scaning phase is initiated
	quantBuffer.clear();		//	Buffers are wiped
	isScaning = true;			//	All flags - lowered
	barcodePending = false;
	canGiveBarcode = false;
	quantityObtained = false;
	quantityAwaited = false;
	incrementRequired = false;
	elementsReadyForExtraction = false;
	barcodeTrigger = high_resolution_clock::now(); // Notes that now is awaited barcode input, so all values must have short delay between them
}

void scanState::pressReturn()
// This method is processing return presses. Most of the times return press means changing phase
{
	if (isScaning)
		// Scaning phase handling
	{
		isScaning = false;				//	Stops scaning phase
		barcodePending = true;			//	This flag is renundant, but remains
		quantityAwaited = true;			//	Sets up Quantity phase: raises Quantity-specific flags
		incrementRequired = true;
		canGiveBarcode = true;			//	Not needed by Quantity, but later will be usefull in Flushing
	}
	else
		// Quantity or flushing phase handling
	{
		quantityObtained = true;	//	Sometimes app is stuck - user did not provided input, but wants to continue. This condition is raises QO flag-> barcode will still
		//	be ignored cause it does not has quantity, but phase change will be possible. most known case: user deleted quantity, that lowered both incReq and QO.
	}
}

void scanState::barcodeElementReleased(QString& elem)
// This method is responsible for automatical routing of non-controlling input
{
#ifdef DEBUG
	detrace_METHEXPL(stateOfEngine());
#endif
		if (isScaning)
			// In Scaning phase
		{
			if (duration_cast<milliseconds>(high_resolution_clock::now() - barcodeTrigger).count() < 80)
				// Values that are provided fast enough are directly written into buffer
			{
				barcodeBuffer += elem;
				barcodeTrigger = high_resolution_clock::now(); // refreshing time point
				return;
			}
			// sometimes user is pressing scan, but barcode is incoming later. This condition is responsible for handling these cases
			barcodeBuffer.clear();
			barcodeBuffer += elem;

			// Time point is refreshed
			barcodeTrigger = high_resolution_clock::now();
		}
		else
			if (quantityAwaited)
				//Quantity phase
			{
				if (duration_cast<milliseconds>(high_resolution_clock::now() - quantityBufferTrigger).count() < 30)
					// All input must be slow enough
				{
#ifdef DEBUG
					detrace_METHEXPL("too short period, clearing buffer");
#endif

					quantityBufferTrigger = high_resolution_clock::now();
					quantityAwaited = true;
					if (quantBuffer.isEmpty())
						return;
					// if quantity buffer was not empty - exists high possibility that one number from barcode already was inserted into buffer
					quantBuffer.chop(1);		//	it must be erased
					return;
				}
				// Ok, it was slow enough. Now conversion to int is required
				bool ok;
				elem.toInt(&ok);
				if (ok)
					// It is number. It can be pushed to buffer
				{
					quantBuffer += elem;
				}
				if (quantBuffer.count() > 0)
					// If buffer has enough values
				{
					quantityObtained = true;	//	this quantity is ok, may proceed to next phase
					incrementRequired = false;	//	increment is not required
					//if (quantBuffer.length() == barcodeBuffer.length())
					//	this renundant piese of code was used to eliminate barcodes.
					//{
					//	if (quantBuffer == barcodeBuffer)
					//	{
					//		quantBuffer = "1";
					//	}
					//}
					// </renundant
				}
				quantityBufferTrigger = high_resolution_clock::now(); // refreshing
			}
			else {
				// if quantity is not awaited, and scanning was not awaited too, then it seems that this is a new barcode interrupting Flushing
				barcodeBuffer.clear();	//	Buffer is cleared and scaning begins
				barcodeBuffer += elem;
				barcodeTrigger = high_resolution_clock::now();
				quantBuffer.clear();		//	Buffers are wiped
				isScaning = true;			//	All flags - lowered
				barcodePending = false;
				canGiveBarcode = false;
				quantityObtained = false;
				quantityAwaited = false;
				incrementRequired = false;
				elementsReadyForExtraction = false;
			}
}

QString scanState::getBarcode()
// This method gets barcode and lowers all flags linked to it. This will be reflected on Flushing phase -> it will be stopped
{
	QString temp = barcodeBuffer;
	barcodeBuffer.clear();
	canGiveBarcode = false;
	barcodePending = false;
	return temp;
}

QString scanState::getQuantity()
// This method gets quantity and lowers all flags linked to it. This will be reflected on Flushing phase
{
	QString temp = quantBuffer;
	quantBuffer.clear();
	quantityAwaited = false;
	quantityObtained = false;
	elementsReadyForExtraction = false;
	return temp;
}

QString scanState::peekQuantity()
{
	return quantBuffer;
}

QString scanState::peekBarcode()
{
	return barcodeBuffer;
}

bool scanState::barcodeReadyForExtraction()
{
	return canGiveBarcode;
}

bool scanState::requiresIncrement()
{
	return incrementRequired;
}

bool scanState::chopQuantity()
// This method removes one number off quality. Possible enhancement-> may drop state to isScaning if there is nothing in quantity buffer
{
	if (quantBuffer.count() > 0)
	{
		quantBuffer.chop(1);
		if (quantBuffer.count() == 0)
		{
			incrementRequired = true;
		}
		return true;
	}
	return false;
}

bool scanState::expectQuantity()
{
	return quantityAwaited;
}

void scanState::forceSetQuantity(int val)
{
	quantBuffer = QString::number(val);
	incrementRequired = false;
}

bool scanState::hasQuantity()
{
	return quantityObtained;
}

bool scanState::canTakeElements()
// Indicator of Flushing phase
{
	return canGiveBarcode && quantityObtained;
}

void scanState::clear()
{
	barcodePending = false;
	quantityAwaited = false;
	canGiveBarcode = false;
	quantityObtained = false;
	incrementRequired = false;
	isScaning = false;
	elementsReadyForExtraction = false;
	barcodeBuffer.clear();
	quantBuffer.clear();
}

bool scanState::isExpectingBarcode()
{
	return isScaning;
}

QString scanState::stateOfEngine()
// Debugging message
{
	QString out;
	QTextStream sout(&out);
	sout << "Now mode is " << ((isScaning) ? "Scaning" : "QuanL") << " state of general flags"
		<< " BP: " << ((barcodePending) ? "pending" : "clear") << " QA: " << ((quantityAwaited)
			? "awaited" : "not awaited") << "\nAnd secondary variables: " << "CGB: " << ((canGiveBarcode) ?
				"can" : "can not") << " QO: " << ((quantityObtained) ? "obtained" : "not obtained") << "\n"
		<< "State of buffer B: " << barcodeBuffer << " State of buffer Q " << quantBuffer << "\n";
	sout.flush();
	return out;
}

void MultistateScanEngine::scaningPhaseProcessing(QString& elem)
// process element during Scaning phase
{
	if (duration_cast<milliseconds>(high_resolution_clock::now() - barcodeTrigger).count() < 80)
		// Values that are provided fast enough are directly written into buffer
	{
		barcodeBuffer += elem;
		barcodeTrigger = high_resolution_clock::now(); // refreshing time point
		return;
	}
	// sometimes user is pressing scan, but barcode is incoming later. This condition is responsible for handling these cases
	barcodeBuffer.clear();
	barcodeBuffer += elem;
	// Time point is refreshed
	barcodeTrigger = high_resolution_clock::now();
}

void MultistateScanEngine::valuesPhaseProcessing(QString& elem)
// process element during Values Awaiting phase
{
	if (duration_cast<milliseconds>(high_resolution_clock::now() - valuesBuffersTrigger).count() < 30)
		// All input must be slow enough
	{
		// seems that someone was scanning barcode while value was awaited. This input must be ignored
#ifdef DEBUG
		detrace_METHEXPL("too short period, clearing buffer");
#endif
		valuesBuffersTrigger = high_resolution_clock::now();
		// if it's okay and fast input did not corrupted buffer - return
		if (valuesBuffer[currentValueIndex].isEmpty())
			return;
		// if quantity buffer was not empty - exists high possibility that one number from barcode already was inserted into buffer
		valuesBuffer[currentValueIndex].chop(1);		//	it must be erased
		return;
	}
	// Ok, it was slow enough. Now conversion to int is required
	bool ok;
	elem.toInt(&ok);
	if (ok)
		// It is number. It can be pushed to buffer
	{
		valuesBuffer[currentValueIndex] += elem;
	}
	if (valuesBuffer[currentValueIndex].count() > 0)
		// If buffer has enough values
	{
		valueRequiresDefault = false; // It is not requires default - value already exists
		if (currentValueIndex >= lastRequired)
		{	
			valuesObtained = true;	//	if this value is over last required value - it indicates that now values can be extracted
			//	but this is not a marker that all values are filled - some can be filled with defaults
		}
	}
	valuesBuffersTrigger = high_resolution_clock::now(); // refreshing
}

void MultistateScanEngine::flushingPhaseProcessing(QString& elem)
// Sometimes user keeps scanning when values are already provided - engine must clear it's state and continue to listen
{
	barcodeBuffer.clear();	//	Buffer is cleared and scaning begins
	barcodeBuffer += elem;
	barcodeTrigger = high_resolution_clock::now();
	clearValBuffer();
	isScaning = true;
	canGiveBarcode = false;
	valuesObtained = false;
	valueAwaited = false;
	valueRequiresDefault = false;
	elementsReadyForExtraction = false;
}

void MultistateScanEngine::clearValBuffer()
// correctly clears buffer with values
{
	valuesBuffer.clear();
	for (int i = 0; i < totalValues; ++i)
	{
		valuesBuffer.push_back("");	//	initialization with empty string
	}
}

MultistateScanEngine::MultistateScanEngine(int valuesToAwait, int LastRequired)
	: isScaning(false), canGiveBarcode(false),
	valueAwaited(false), valuesObtained(false),
	elementsReadyForExtraction(false), valueRequiresDefault(false),
	totalValues(valuesToAwait), currentValueIndex(0),
	valuesBuffer(valuesToAwait), lastRequired(LastRequired), barcodeBuffer(),
	lastPress(), valuesBuffersTrigger(), barcodeTrigger()
{
	valuesBuffer.resize(valuesToAwait);
}

void MultistateScanEngine::pressScan()
// Main interface function - provides scan button reaction
{
	elementsReadyForExtraction = canGiveBarcode && valuesObtained;	//	Refreshes this flag to be sure that it reflects actual state
	if (elementsReadyForExtraction)		//	if they are ready, phase is changed to Flushing and quantity listening is stopped.
	{
		valueAwaited = false;
		return;	//	elements must be extracted
	}
	if (isScaning || currentValueIndex < lastRequired)		//	if not all values were already obtained or we already awaiting barcode - return
		return;
	barcodeBuffer.clear();		//	If it is not Flushing - new Scaning phase is initiated
	clearValBuffer();
	isScaning = true;			//	All flags - lowered
	canGiveBarcode = false;
	valuesObtained = false;
	valueAwaited = false;
	valueRequiresDefault = false;
	elementsReadyForExtraction = false;
	currentValueIndex = 0;
	barcodeTrigger = high_resolution_clock::now(); // Notes that now is awaited barcode input, so all values must have short delay between them
}

void MultistateScanEngine::pressReturn()
// Main interface - provides return button reaction
{
	if (isScaning)
		// Scaning phase handling
	{
		if (barcodeBuffer.isEmpty())
			return;
		isScaning = false;				//	Stops scaning phase
		valueAwaited = true;			//	Sets up Quantity phase: raises Quantity-specific flags
		currentValueIndex = 0;
		canGiveBarcode = true;			//	Not needed by Quantity, but later will be usefull in Flushing
		return;
	}
	else if (valueAwaited)
	{
		// if value iwas awaited -> proceed to next one
		++currentValueIndex;
		if (currentValueIndex >= totalValues)
		{
			// If there is no next one: prepare Flushing phase
			valuesObtained = true;
			valueAwaited = false;
			valueRequiresDefault = false;
			elementsReadyForExtraction = true;
			--currentValueIndex;
		}
		if (currentValueIndex >= lastRequired && valuesBuffer[currentValueIndex].isEmpty())
		{
			// if current value is empty but not required - raise default flag
			valueRequiresDefault = true;
		}
		else {
			valueRequiresDefault = false;
		}
	}
}

void MultistateScanEngine::processNonControlInput(QString& inp)
// Main interface - reacts to non flow control buttons
{
#ifdef DEBUG
	detrace_METHEXPL(stateOfEngine());
#endif
	// Due to large amount of code, all algorythms were moved to independent functions
	if (isScaning)
		// In Scaning phase
	{
		scaningPhaseProcessing(inp);
	}
	else
		if (valueAwaited)
			//Quantity phase
		{
			valuesPhaseProcessing(inp);
		}
		else {
			// if quantity is not awaited, and scanning was not awaited too, then it seems that this is a new barcode interrupting Flushing
			flushingPhaseProcessing(inp);
		}
}

QString MultistateScanEngine::getBarcode()
// returns barcode and erases buffer
{
	QString temp = barcodeBuffer;
	barcodeBuffer.clear();
	canGiveBarcode = false;
	return temp;
}

QString MultistateScanEngine::peekCurrentValue()
// returns current value
{
	return valuesBuffer.at(currentValueIndex);
}

QString MultistateScanEngine::peekBarcode()
//returns barcode
{
	return barcodeBuffer;
}

QString MultistateScanEngine::peekValueAt(int ind)
// returns value under index
{
	if (ind < 0 || ind >= totalValues)
		return "";
	return valuesBuffer.at(ind);
}

bool MultistateScanEngine::barcodeReadyForExtraction()
// true if can give barcode
{
	return canGiveBarcode;
}

bool MultistateScanEngine::isAwaitingBarcode()
{
	return isScaning;
}

bool MultistateScanEngine::chopCurrentValue()
// true if chopping was succesfull
{
	if (!valuesBuffer[currentValueIndex].isEmpty())
	{
		valuesBuffer[currentValueIndex].chop(1);
		if (valuesBuffer[currentValueIndex].count() == 0)
		{
			valueRequiresDefault = true;
		}
		return true;
	}
	return false;
}

bool MultistateScanEngine::isExpectingValue()
// true if value is awaited
{
	return valueAwaited;
}

void MultistateScanEngine::forceSetValue(int v)
// overwrites current value with new one
{
	valuesBuffer[currentValueIndex] = QString::number(v);
	if (valuesBuffer[currentValueIndex].isEmpty())
	{
#ifdef DEBUG
		detrace_METHEXPL("setting forced value " << v << " while state " << stateOfEngine());
#endif
		// If no value was provided - flag is raised no matter the lastRequired
		valueRequiresDefault = true;
	}
}

bool MultistateScanEngine::hasAllValues()
// returns if current value is last one. Does not tells if last value is completed
{
	return currentValueIndex == totalValues - 1;
}

bool MultistateScanEngine::canTakeElements()
// return if it is a flushing phase
{
	return elementsReadyForExtraction;
}

void MultistateScanEngine::clear()
// drops engine state to zero - it is equal to recreating engine
{
	valueAwaited = false;
	canGiveBarcode = false;
	valuesObtained = false;
	valueRequiresDefault = false;
	isScaning = false;
	elementsReadyForExtraction = false;
	barcodeBuffer.clear();
	clearValBuffer();
	currentValueIndex = 0;
}

int MultistateScanEngine::getCurrentValueIndex()
// return index of current value
{
	return currentValueIndex;
}

bool MultistateScanEngine::currentValueRequiresDefault()
// returns if current value can be replaced with default
{
	return (currentValueIndex < lastRequired) ? false : valueRequiresDefault;
}

QVector<int> MultistateScanEngine::getConvertedValues()
// returns vector with converted values and erases buffer
{
	auto temp = getValues();
	QVector<int> toret;
	auto start = temp.begin();
	while (start != temp.end())
	{
#ifdef DEBUG
		detrace_CYCLEEXPL("value " << start->toInt() << " obtained from " << *start);
#endif

		toret << start->toInt();
		++start;
	}
	return toret;
}

QVector<QString> MultistateScanEngine::getValues()
// returns values and erases buffer
{
	QVector<QString> temp;
	auto start = valuesBuffer.begin();
	while (start != valuesBuffer.end())
	{
		temp.push_back(*start);
		++start;
	}
	clearValBuffer();
	valueAwaited = false;
	valuesObtained = false;
	elementsReadyForExtraction = false;
	return temp;
}

QString MultistateScanEngine::stateOfEngine()
// returns debugging string describing current state of engine
{
	QString out;
	QTextStream sout(&out);
	sout << "Now phase is " << ((isScaning) ? "Scaning" : ((valueAwaited) ? "ValueAwaiting" : "Flush")) << " state of general flags"
		<< " Barcode: " << ((canGiveBarcode) ? "exists" : "empty") << " Value: " << ((valueAwaited)
			? "awaited" : "not awaited") << "\nAnd secondary variables: " << "Values : " << ((elementsReadyForExtraction) ?
				"can" : "can not") << " be extracted, values: " << ((valuesObtained) ? "obtained" : "not obtained") << "\r\n"
		<< "counter state: " << currentValueIndex << " is current, " << totalValues << " is total\r\n"
		<< "State of buffer B: " << barcodeBuffer << " State of val buffer " << "\r\n";
	for (int i = 0; i < valuesBuffer.count(); ++i)
	{
		sout << QString::number(i) << " : " << valuesBuffer.at(i) << "\r\n";
	}
	sout.flush();
	return out;
}