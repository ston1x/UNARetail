#include "SerializationSettings.h"
#include "widgets/utils/GlobalAppSettings.h"
#ifdef Q_OS_ANDROID
#include <qscroller.h>
#endif


SerializationSettings::SerializationSettings(Entity Prototype, QWidget* parent)
	: QWidget(parent), mainLayout(new QVBoxLayout(this)), fieldPicker(new QListWidget(this)),
	fieldsModel(Prototype->getFields()), prototype(Prototype)
{
	setLayout(mainLayout);
	mainLayout->addWidget(fieldPicker);
	fieldPicker->addItems(fieldsModel);
	fieldPicker->setSelectionMode(QAbstractItemView::MultiSelection);
	fieldPicker->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
#ifdef Q_OS_ANDROID
	QScroller::grabGesture(fieldPicker, QScroller::LeftMouseButtonGesture);
#endif
	QList<int> & toSerialize =  AppSettings->serializationOrder[prototype->myType()];
	for (int i = 0; i < toSerialize.count(); ++i)
	{
		fieldPicker->setItemSelected(fieldPicker->item(toSerialize.at(i)), true);
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
	AppSettings->serializationOrder[prototype->myType()] = toSerialize;

}
