#include "SerializationSettings.h"
#include "widgets/utils/GlobalAppSettings.h"
#ifdef Q_OS_ANDROID
#include <qscroller.h>
#endif


SerializationSettings::SerializationSettings(Modes mode, Entity Prototype, QWidget* parent)
	: QWidget(parent), mainLayout(new QVBoxLayout(this)), fieldPicker(new QListWidget(this)),
	fieldsModel(Prototype->getFields()), prototype(Prototype), currentMode(mode)
{
	setLayout(mainLayout);
	mainLayout->addWidget(fieldPicker);
	fieldPicker->addItems(fieldsModel);
	fieldPicker->setSelectionMode(QAbstractItemView::MultiSelection);
	fieldPicker->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
#ifdef Q_OS_ANDROID
	QScroller::grabGesture(fieldPicker, QScroller::LeftMouseButtonGesture);
#endif
	const QList<int>& toSerialize = AppSettings->getModeDescription(currentMode).getSerializationOrder();
	if (toSerialize.isEmpty())
	{
		fieldPicker->selectAll();
	}
	else
	{
		for (int i = 0; i < toSerialize.count(); ++i)
		{
			fieldPicker->setItemSelected(fieldPicker->item(toSerialize.at(i)), true);
		}
	}
}
void SerializationSettings::extractAndSave()
{
	QList<int> toSerialize;
	QList<QListWidgetItem* > items = fieldPicker->selectedItems();
	QList<QListWidgetItem*>::iterator begin = items.begin();
	while (begin != items.end())
	{
		if (*begin != Q_NULLPTR)
			toSerialize << fieldPicker->row((*begin++));
	}
	AppSettings->getModeDescription(currentMode).setSerializationOrder(toSerialize);

}
