#pragma once

struct BarcodeUniform
{
	int asInt;				//	int value - for quicker search. Currently not used
	QString asString;		//	string value - used everywhere.
	unsigned int quantity;	//	quantity of times barcode was scanned
	QDateTime addDate;		//	datetime when barcode was scanned. If it was scanned multiple times - only first stays
	QString comment;		//	comment to barcode
};