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
	enum mainPageWidgets { Inventory, Supplies, Search, Simple, Camera, Prices };
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
#ifdef CAMERA_SUPPORT
	IndexedButton* camera;
#endif
	IndexedButton* prices;
	QHBoxLayout* controlPanel;			//	This overlay is always enabled on any widget
	IgnorantButton* exitButton;			//	quits app
	MegaIconButton* settingsButton;		//	opens settings screen of current child branch
	bool lock;
public:
	CoreWidget(QWidget* parent = nullptr);

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

/*

		HOW TO USE CURRENT

		assign new current widget with it's address::
				current = &mainPage
		access current widget by * operator:
				(*current)->hide()
		check inner pointer for null:
				(*current).isNull()
		when creating a new widget - check if it is not already created by isNull.
		when you need to access child signals: make three steps casting:
			if (!mainPage.isNull())
			{
				auto temp = qobject_cast<MainPageWidget*> (mainPage);
				if (temp != nullptr)
				{
					connect(temp, signal etc...
				}
			}

*/