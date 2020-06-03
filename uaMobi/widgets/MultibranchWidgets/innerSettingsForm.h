#pragma once
#include "widgets/parents/inframedWidget.h"
#include "dataproviders/ModesDescriptions.h"
/*

		This class is providing special settings form for currentrly available branches.
		TODO : add some kind of serialization - by db or by file.

		Signals: dropScannedReceived : emitted when user requests all data wipe.

		Update: created some access methods.
		Update: added dropDataReceived signal - emitted when user requests for deleting all data
		Update: buttons now are MegaIcon and styled - check ElementsStyles for additional info.
			Minor changes to delete all slots : now with confirmation dialog
		Update: keyfilter now applied to catch line-edits back key while keeping their original
		reactions.
*/
class QVBoxLayout;
class QTabWidget;
class ScaningSettings;
class DatabaseSettings;
class ExtraScaningSettings;
class MegaIconButton;
class TaxInvoiceSettings;
class innerSettingsForm :
	public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QTabWidget* innerWidget;
	ScaningSettings* scaningSettings;
	DatabaseSettings* databaseSettings;
	ExtraScaningSettings* extrascanSettings;
	TaxInvoiceSettings* taxInvoiceSettings;

	MegaIconButton* backButton;
public:
	innerSettingsForm(Modes mode, QWidget* parent);
protected slots:
	void dumpAndReturn();
};
