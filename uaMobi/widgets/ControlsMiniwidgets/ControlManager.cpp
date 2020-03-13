#include "ControlManager.h"
#include "QuantityControl.h"
#include "DateTimeControl.h"
#include "LabelControl.h"
#include "BarcodeControl.h"
#include "StringControl.h"



abs_control* fabricateControl(InputControl control, QBoxLayout* layout, QWidget* parent)
{
	switch (control->getAttachedNumber())
	{
	case InputControlEntity::Float:
	{
		QuantityControl* qc = new QuantityControl(false, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		qc->setValue(control->defaultValue);
		return qc;
	}
	case InputControlEntity::Int:
	{
		QuantityControl* qc = new QuantityControl(true, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		qc->setValue(control->defaultValue);
		return qc;
	}
	case InputControlEntity::Date:
	{
		DateTimeControl* dc = new DateTimeControl(false, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, dc->myWidget());
		dc->setValue(control->defaultValue);
		return dc;
	}
	case InputControlEntity::Label:
	{
		LabelControl* lc = new LabelControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, lc->myWidget());
		lc->setValue(control->defaultValue);
		return lc;
	}
	case InputControlEntity::Barcode:
	{
		BarcodeControl* bc = new BarcodeControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, bc->myWidget());
		bc->setValue(control->defaultValue);
		return bc;
	}
	case InputControlEntity::String:
	{
		StringControl* sc = new StringControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, sc->myWidget());
		sc->setValue(control->defaultValue);
		return sc;
	}
	case InputControlEntity::None:
	default:
		return Q_NULLPTR;
	}
}
