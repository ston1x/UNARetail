# UNARetail Documentation

## Building and preparing

This project is fully prepared for building on old Qt down to 4.8 and building on new Qt versions up to newest (5.13.2).  To disable compatibility mode (required for Win CE and Win XP builds) define QT_VERSION5X.

### Usable flag definitions:

QT\_VERSION5X : enables and disables Qt5 only features and includes. Must be defined if your qt version is not 4x. Also enables C++ 11 standard.

Q\_NULLPTR	"0": this constant must be enabled for QT4 because old cpp standard is lacking _nullptr_.

DEBUG	    : this flag enables debugging. It will print all usefull data about program flow into logs. Warning - do not enable this in release version to avoid too big log files.

CAMERA_SUPPORT     : this flag enables QML camera support. Do not enable it on Qt4 because of QML incompatibility. 

### Library and enviroments required

For modern systems:

QtCreator OR

Visual Studio with Qt Visual Tools extension and connected QT version.

Android:

NDK 19-20+ (tested and compiled on 20.55)

SDK 26+ (tested and compiled on 26)

Windows:

Microsoft Visual Studio 2017-19 (compiled on v.142)
Windows SDK 10.0

 For older systems:

QtCreator 3+ and any installed Qt4.8+ library. Use UamobiUtilCE package because of incompatibility with vs2005.

WinCE5.0\6.0

Microsoft Visual Studio 2005

platform supported SDK (usually Windows Mobile SDK 5.0 is fine). If possible, use platform-provided SDK like HONEYWELL D6X00. 

Qt 4 binaries for this platform. Note that it is impossible to just google them. Options:

Virtual machine winxp_dev contains all binaries for wince5\6 in D:\Dev

You still can try building Qt for required platform. Sources are located on same VM. 

### Building step by step

 Qt4 for QtCreator. WinCe/WinXp


* Open Qt Creator. Export project by opening UNARetailCE.pro file.
     * Do not forget, that you must used different project files for different systems
* Assign building kit for the project. Use msvc2005\2008 for WinXp and platform-provided sdk for wince.
* Check flags in .pro file. Set INCLUDEPATH += {your qt destination}. Disable QT_VERSION5X.
* Build. Do not forget that for proper work you will need visual studio redistributable for your compiler. 

Qt5 for Visual Studio

* Open Visual Studio 2017+
    * Install Visual Studio Qt Tools extension
    * Add proper version of Qt binaries
    * In "project settings" add your qt version destination. 
    * Check modules _GUI, CORE, WIDGETS, NETWORK, XML_
* Check flags in C++ compiler settings. Enable QT_VERSION5X
* Build. Do not forget to launch windeployqt.exe after build. 

Qt5 for QtCreator. Android etc.

* Open QtCreator. Export project file.
* Assign building kit for target platform
    * For android: download Android Studio. In SDK settings select SDK\NDK tools.
	* Do not forget to add paths to JRE, NDK at DEVICE settings.
    	* Do not forget to choose correct ARM version. Usually armeabi_v7a is ok, sometimes arm64 v8 is required. x86 is rarely used.
* Build.

# Main construction and architecture notes:

This application was concentrated over full crossplatform compatibility. That is why I discourage you of using platform-dependent libraries like <windows.h>. If you desperately need something - hide it under #ifdef.  
This application is based on sqlite database. This application can be autonomic. 

## Structure

This application is concentrated over branching structure with static behaviour. It has some basic nodes which can be recombined via recompilation. Each node is considered independent.

__inframed widget__  
This class is the wrapper for normal QWidget, which provides some usefull interfaces, and has own keyfilter that is usefull for key-navigation over application. It is rarely required.

__Global Settings Object__  
This singletone object stores data used through all the application.

__Element Styles__  
In this project is used fully static definition of stylesheets. Header _ElementsStyles.h_ is defining them, and is providing scaling functions. They are creating fonts and sizes using available geometry of main screen.  

__Entities__  
For handling inner data were created some classes wrapping datasets and frequently used queries. You can use them polymorthically to avoid multiple realisations of list views.
Also these entities are providing tables associated with them, which allows fast and uniform realisation of most frequently used SQL queries.

__Controls__
This project inherits control system from the other project. It's interface provides polymorthic access to input elements, while most of it's functionality is not required. Still, they have a lot of usefull features and bugfixes.
