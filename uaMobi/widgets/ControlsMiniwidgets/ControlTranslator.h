#pragma once
#include <qobject.h>


/*
		This file contains static translator which is translating controls codenames
		into app language. Do not create new CTranslator's, it is pointless. This object 
		is empty wrapper with only method mapping codenames to translations. Use instead
		makeControlTranslation to call static object - this is faster.
*/


class ControlTranslator : private QObject
{
	Q_OBJECT
public:
	ControlTranslator();
	QString translate(QString&);
};


QString makeControlTranslation(QString&);