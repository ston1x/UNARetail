#pragma once
#include "SettingsForm.h"
#include "widgets/SupplyBranch/SuppliesWidget.h"
#include "widgets/InventoryBranch/InventoryWidget.h"
#include "widgets/SimpleBranch/SimpleBranchWidget.h"
#ifdef CAMERA_SUPPORT
#include "widgets/CameraBranch/CameraWidget.h"
#endif
#include "widgets/SearchBranch/SearchWidget.h"
#include "dataproviders/sqldataprovider.h"
#include <QKeyEvent>
#include <QtCore/QPointer>
#include <QGridLayout>
#include "widgets/parents/abstractNodeInterface.h"


class IndexedButton : public MegaIconButton
{
	Q_OBJECT
private:
	int index;
public:
	IndexedButton(int index, QWidget* parent);
private slots:
	void clickReceived();
signals:
	void iclicked(int index);
};
namespace mpw {
	enum mainPageWidgets { Inventory, Supplies, Search, Simple, Invoice, Prices };
}

class CoreWidget : public QWidget, abstractDynamicNode
{
	Q_OBJECT
private:
	QGridLayout* innerLayout;
	IndexedButton* inventory;
	IndexedButton* supplies;
	IndexedButton* search;
	IndexedButton* simple;
	IndexedButton* invoice;
	IndexedButton* prices;
	QHBoxLayout* controlPanel;			
	IgnorantButton* exitButton;			//	quits app
	MegaIconButton* settingsButton;		//	opens settings screen 
	bool lock;
public:
	CoreWidget(QWidget* parent = Q_NULLPTR);

public slots:
	void settingsPressed();
	void exitPressed();
	void hideCurrent();
	void branchRequired(int number);
	void allocateLock();
	void deallocateLock();
	void handleBack();
	void retranslate();
	void refreshFonts();
signals:
	void requestSettings();
};

