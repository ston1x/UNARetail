
#-------------------------------------------------
#
# Project created by QtCreator 2019-08-05T10:17:07
#
#-------------------------------------------------

QT       += core gui sql network xml multimedia multimediawidgets quickwidgets qml quick

!win32:QT += androidextras
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += qzxing_qml
CONFIG += qzxing_multimedia
include(qzxing-master/src/qzxing.pri)

TARGET = uaMobi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    dataFormats/csvformat.cpp \
    dataFormats/dataformatcore.cpp \
    dataFormats/dataformater.cpp \
    dataFormats/jsonformat.cpp \
    dataFormats/xmlformat.cpp \
    dataproviders/abstractdataprovider.cpp \
    dataproviders/sqldataprovider.cpp \
    debugtrace.cpp \
    externalCommunication/communicationCore.cpp \
    externalCommunication/tohttp.cpp \
    externalCommunication/tolocalmemory.cpp \
    externalCommunication/totcpip.cpp \
    helperClasses/appsettings.cpp \
    widgets/CameraBranch/CameraWidget.cpp \
    widgets/CameraBranch/MyVideoFilter.cpp \
    widgets/CameraBranch/MyVideoFilterRunnable.cpp \
    widgets/CoreWidget.cpp \
    widgets/InventoryBranch/InventoryWidget.cpp \
    widgets/MainPage/MainPageWidget.cpp \
    widgets/MultibranchWidgets/BarcodeRedactingWidget.cpp \
    widgets/MultibranchWidgets/ReceiveWidget.cpp \
    widgets/MultibranchWidgets/ScanedStorageWidget.cpp \
    widgets/MultibranchWidgets/ScaningWidget.cpp \
    widgets/MultibranchWidgets/innerSettingsForm.cpp \
	widgets/MultibranchWidgets/SendingDataPickerWidget.cpp\
	widgets/MultibranchWidgets/SendSettings.cpp\
    widgets/SearchBranch/SearchWidget.cpp \
    widgets/SettingsForm.cpp \
    widgets/SimpleBranch/SimpleBranchWidget.cpp \
    widgets/SupplyBranch/SuppliesWidget.cpp \
	widgets/PriceBranch/PriceBranchWidget.cpp\
	widgets/PriceBranch/PricedBarcodeRedacting.cpp\
	widgets/PriceBranch/PriceScaningWidget.cpp\
    widgets/parents/BranchRootWidget.cpp \
	widgets/parents/AbstractScaningWidget.cpp\
	widgets/parents/AbstractCameraWidget.cpp\
    widgets/utils/MegaIconButton.cpp \


HEADERS += \
    dataFormats/csvformat.h \
    dataFormats/dataformatcore.h \
    dataFormats/dataformater.h \
    dataFormats/jsonformat.h \
    dataFormats/xmlformat.h \
    dataproviders/abstractdataprovider.h \
    dataproviders/sqldataprovider.h \
    debugtrace.h \
    externalCommunication/communicationCore.h \
    externalCommunication/tohttp.h \
    externalCommunication/tolocalmemory.h \
    externalCommunication/totcpip.h \
    helperClasses/appsettings.h \
    widgets/CameraBranch/CameraWidget.h \
    widgets/CameraBranch/MyVideoFilter.h \
    widgets/CameraBranch/MyVideoFilterRunnable.h \
    widgets/CoreWidget.h \
    widgets/InventoryBranch/InventoryWidget.h \
    widgets/MainPage/MainPageWidget.h \
    widgets/MultibranchWidgets/BarcodeRedactingWidget.h \
    widgets/MultibranchWidgets/ReceiveWidget.h \
    widgets/MultibranchWidgets/ScanedStorageWidget.h \
    widgets/MultibranchWidgets/ScaningWidget.h \
    widgets/MultibranchWidgets/innerSettingsForm.h \
	widgets/MultibranchWidgets/SendingDataPickerWidget.h\
	widgets/MultibranchWidgets/SendSettings.h\
    widgets/SearchBranch/SearchWidget.h \
    widgets/SettingsForm.h \
    widgets/SimpleBranch/SimpleBranchWidget.h \
    widgets/SupplyBranch/SuppliesWidget.h \
	widgets/PriceBranch/PriceBranchWidget.h\
	widgets/PriceBranch/PricedBarcodeRedacting.h\
	widgets/PriceBranch/PriceScaningWidget.h\
    widgets/parents/BranchRootWidget.h \
	widgets/parents/AbstractScaningWidget.h\
	widgets/parents/AbstractCameraWidget.h\
    widgets/parents/inframedWidget.h \
    widgets/utils/BarcodeUniform.h \
    widgets/utils/ElementsStyles.h \
    widgets/utils/GlobalAppSettings.h \
    widgets/utils/MegaIconButton.h \
    widgets/utils/UniformBarcode.h \


CONFIG += mobility
MOBILITY = 


QML_IMPORT_PATH = D:\Lib\5.12.2
QML_DESIGNER_IMPORT_PATH = D:\Lib\5.12.2

RESOURCES += resources.qrc

TRANSLATIONS += uamobi_ru.ts uamobi_ro.ts uamobi_en.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_LINK += -nostdlib++

DISTFILES += \
    CameraSurface.qml \
    android-sources/AndroidManifest.xml \
    android-sources/libs/core-3.3.0.jar \
    android-sources/libs/javase-3.3.0.jar \
    android-sources/libs/zxing-integration.jar \
    android-sources/res/drawable-hdpi/icon.png \
    android-sources/res/drawable-ldpi/icon.png \
    android-sources/res/drawable-mdpi/icon.png \
    android-sources/src/com/ovi/UAMobiOffline/Scanner.java \
    android-sources/src/com/ovi/UAMobiOffline/UAMobiOffline.java
