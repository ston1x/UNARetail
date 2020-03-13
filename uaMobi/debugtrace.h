#pragma once
#include <QTextStream>
#include <QFile>
#include <QString>
#include <exception>
#include <QDebug>
#include <iostream>
#include <QVector>
#include <QFile>
#include <iomanip>
/*
	This is debugging and tracing module. You can use it for dynamic testing
	with varying verbosity of this tool. Use macroses starting with detrace_
	to automatically output your data. Alter creation of main detrace object to
	set up output mode and filter of the messages.
	use setp to set up priority for incomming input.

	TODO: add filtering by filenames to skip messages from unnecessary headers.

	Update:
		+1 macro
	Update:
		+1 macro
		replaced comments so they were captured with msvs
	Update:
		better naming of macros variables
  */
#ifdef Q_OS_ANDROID
#define FOUTPATH "/storage/emulated/0/UNALogs/log.txt"
#define ERROPATH "/storage/emulated/0/UNALogs/errlog.txt"
#define NETOPATH "/storage/emulated/0/UNALogs/netlog.txt"
#endif
#ifdef Q_OS_WIN32
#define FOUTPATH "UNALogs/log.txt"
#define ERROPATH "UNALogs/errlog.txt"
#define NETOPATH "UNALogs/netlog.txt"
#endif
#ifdef Q_OS_WINCE
#define FOUTPATH "UNALogs/log.txt"
#define ERROPATH "UNALogs/errlog.txt"
#define NETOPATH "UNALogs/netlog.txt"
#endif



  // print message about creating an object within specified method or function
#define detrace_OCREATED(OBJECT, AROUND) debugtrace::getObject() << detr_supply::objectConstruction \
    << "][\tCreated object " << OBJECT << " around " << AROUND << detr_supply::Actions::endl

  // print message about triggered default constructor
#define detrace_DCONSTR(OBJECT) debugtrace::getObject() << detr_supply::defaultConstructor << \
    "][\tCreated object " << OBJECT << " on address " << ((unsigned long long int)this) <<  detr_supply::Actions::endl

// pring message about something happening in method
#define detrace_METHEXPL(HAPPENED) debugtrace::getObject() << detr_supply::methodExplanation << HAPPENED << detr_supply::Actions::endl

// print message about something not important
#define detrace_SIMPLMSG(MESSAGE) debugtrace::getObject() << detr_supply::notImportantMessage << MESSAGE << detr_supply::Actions::endl

// print message about called method
#define detrace_METHCALL(_METHOD_) debugtrace::getObject() << detr_supply::methodCalled << \
    "-->\tMethod called: "  << _METHOD_ << " in object " <<  ((unsigned long long int )this) << detr_supply::Actions::endl

// print message about slot activation
#define detrace_SLOTCALL(_SLOT_) debugtrace::getObject() << detr_supply::methodCalled << "-->\tSlot " \
    << _SLOT_ << " of object "  << ((unsigned long long int )this) << " called" << detr_supply::Actions::endl

// print message about invocation of any method by any method of any class
#define detrace_METHINVOK(INVOCATED_METHOD, _METHOD_ ,_OBJECT_PTR_) debugtrace::getObject() << detr_supply::methodInvocation \
    << "-->\tMethod " << INVOCATED_METHOD << " of object " << ((unsigned long long int) _OBJECT_PTR_) << \
	" invocated in method " << _METHOD_ << " of object " << ((unsigned long long int )this) << detr_supply::Actions::endl

// print message about emitting signal
#define detrace_SIGNALINV(_SIGNAL_, OBJECT) debugtrace::getObject() << detr_supply::signalInvocation\
    << "--!\tSignal " << _SIGNAL_ << " is emitted from " << OBJECT << detr_supply::Actions::endl

// print message containig snapshot of current data. Variables to dump must be provided using <<
#define detrace_METHDATAS(_METHOD_, _VARIABLES_, _STATE_) debugtrace::getObject() << detr_supply::methodDataSnapshot\
    << "[&&&]\tin method " << _METHOD_ << " state of variables "<< _VARIABLES_  << " was " _STATE_ << detr_supply::Actions::endl

// print message containing one text variable and its value in separated place
#define detrace_METHTEXTS(_METHOD_,_VARIABLE_,_TEXT_) debugtrace::getObject() << detr_supply::methodDataSnapshot\
    << "[&&&]\tin method " << _METHOD_ << " state of text variable " << _VARIABLE_ << " was: " << detr_supply::Actions::endl\
    << "\t________________________________________" << detr_supply::Actions::endl << "\t" << debugtrace::shiftRight(_TEXT_)<< detr_supply::Actions::endl

// print message about received arguments in method
#define detrace_METHDRECEIVE(_METHOD_, _ARGUMENTS_ , _VALUES_) debugtrace::getObject() << detr_supply::methodDataSnapshot\
    << "-->[&&&]\tmethod " << _METHOD_ << " received data: arguments " << _ARGUMENTS_ << " were filled with values"\
    << detr_supply::Actions::endl << "\t|" << _VALUES_ << "|" << detr_supply::Actions::endl

// print message about sent request
#define detrace_NETREQSENT(_METHOD_,_REQUEST_, _SUPPLIED_) debugtrace::getObject() << detr_supply::netrequestSent << \
    ">>\t" << makeMsgId() << " method " << _METHOD_ << " sent request " << _REQUEST_ << " supplied with " << _SUPPLIED_ << detr_supply::Actions::endl

#define detrace_NETREQUEST(_URL_) debugtrace::getObject() <<detr_supply::netrequestSent << \
    ">>\t" << makeMsgId() << " sent request: " << _URL_ << detr_supply::Actions::endl

#define detrace_NETRESPARR(_DATA_, _ERR_, _DELIVER_) debugtrace::getObject() << detr_supply::netresponseReceived << \
	"<<\t" << makeMsgId() << " received response going to " << _DELIVER_ << " containing text:|" << detr_supply::Actions::endl << \
     '\t' << debugtrace::compressNetMsg(_DATA_) << detr_supply::Actions::endl << "\tand error text |" \
	<< debugtrace::compressNetMsg(_ERR_) << detr_supply::Actions::endl

#define detrace_NETERROR(_ERROR_, _INFO_) debugtrace::getObject() << detr_supply::netErrorPossible \
    << "> ! <\t" << makeMsgId() << " error making request " <<  _INFO_  << " : " << _ERROR_ << detr_supply::Actions::endl

#define detrace_NRESPERR(_ERROR_) debugtrace::getObject() << detr_supply::netErrorPossible\
    << "> ! <\t" << makeMsgId() << " error receiving response: " << _ERROR_ << detr_supply::Actions::endl

// prints message using high priority
#define detrace_MSGIMP(_MESSAGE_) debugtrace::getObject() << detr_supply::importantMessage << detr_supply::Actions::endl <<\
    "!\t" <<  detr_supply::Actions::endl<< _MESSAGE_ <<"!" << detr_supply::Actions::endl << detr_supply::Actions::endl

// prints low priority message that frequent method was invoked
#define detrace_METHFRECALL(_METHOD_) debugtrace::getObject() << detr_supply::frequentMethodCalled <<\
    "***\tmethod " << _METHOD_ << "called" << detr_supply::Actions::endl

// prints low priority message about cycle
#define detrace_CYCLEEXPL(_EXPLANATION_) debugtrace::getObject() << detr_supply::cycleExplanation << \
    "***\tcycling: " << _EXPLANATION_ << detr_supply::Actions::endl

// prints status of connecting with high priority to avoid disconnection errors
#define detrace_CONNECTSTAT(_SIGNAL_TO_SLOT_, _STATE_) debugtrace::getObject() << detr_supply::errorPossible << \
	"-][-\tafter connecting " << _SIGNAL_TO_SLOT_ << " state was " << _STATE_ << detr_supply::Actions::endl

//print message about possible error during flow of method X
#define detrace_METHPERROR(_METHOD_, _CONTEXT_) debugtrace::getObject() << detr_supply::errorPossible << \
	"-->!<--\tError possible in object " << ((unsigned long long int ) this ) << \
	" in method " << _METHOD_ << "with context: " << _CONTEXT_ << detr_supply::Actions::endl

#define detrace_SUCCESS debugtrace::getObject() << detr_supply::methodDataSnapshot << " (V) Operation success!" << detr_supply::Actions::endl

#define detrace_FAIL debugtrace::getObject() << detr_supply::methodDataSnapshot << " (X) Operation failed!" << detr_supply::Actions::endl

#define detrace_NODECREATED(_TYPE_, _DESCRIPTION_) debugtrace::getObject() << detr_supply::objectConstruction \
    << "]N[\tNode of type " << _TYPE_ << " fabricated using description: " << _DESCRIPTION_ << detr_supply::Actions::endl

#define detrace_APPSTART(_VERSION_, _SUFFIX_) debugtrace::getObject() << detr_supply::netErrorPossible \
	<< "Application start begins: version " << _VERSION_  << " " << _SUFFIX_ << detr_supply::Actions::endl

#define detrace_MODEINTERPRET(_NAME_,_MODE_, _DESCRIPTION_) debugtrace::getObject() << detr_supply::importantMessage \
	<< "[!!!]\tMode named " << _NAME_ << " signed as " << _MODE_ << " received, interpreting: " << detr_supply::Actions::nolimit \
	<<  debugtrace::shiftRight(_DESCRIPTION_) << detr_supply::Actions::endl


namespace detr_supply { // Holds enums for defining output method and priorities
	enum OutputMode { toall, file, qDeb, qStr, Cons, some_united, buffConsole };
	// Defines output mode: file-> prints to log.txt, qDeb-> prints to console,
	// qStr-> prints to inner string, Cons -> prints to std::cout
	enum DebugPriority {
		all, errorPossible, netErrorPossible, importantMessage,
		objectConstruction, methodInvocation,
		signalInvocation,
		methodCalled, methodDataSnapshot, methodExplanation,
		netrequestSent, netresponseReceived, defaultConstructor, frequentMethodCalled,
		cycleExplanation, notImportantMessage
	};
	// Defines priority level: everything that is lower than detrace starting priority
	// will be printed.
	enum class Actions { Flush, endl, ignore, nolimit};
}

using namespace detr_supply;
class debugtrace
	// This class implements priority tracking and prints into various
	// output
{
private:
	struct impl* pimpl;

	static debugtrace* _instanse;
	         //  holds modes for printToSome
	// output method
	void printToFile(const QString& str); // Output methods
	void printToQDebug(const QString& str);
	void printToString(const QString& str);
	void printToAll(const QString& str);
	void printToConsole(const QString& str);
	void printToSome(const QString& str);
	void printToConsBuff(const QString& str);
	bool isValid();
	void flushQDebug();
	void flushBuffer();

public:
	debugtrace(DebugPriority priority, OutputMode mode,
		QVector<OutputMode> OutputTo = QVector<OutputMode>(), int maxMsgLen = 60, bool notMixing = true); // no default values
	debugtrace& operator<<(const DebugPriority); // sets priority for all next
	// messages
	debugtrace& operator<<(const int); // wrappers for normalizing the message
	debugtrace& operator<<(const long long int);
	debugtrace& operator<<(const char*);
	debugtrace& operator<<(const QString&);
	debugtrace& operator<<(const bool);
	debugtrace& operator<<(const double);
	debugtrace& operator<<(const std::exception&);
	debugtrace& operator<<(const char);
	debugtrace& operator<<(quint64);
	debugtrace& operator<<(Actions);
	void changeOutputMode(const OutputMode, QVector<OutputMode>); // changes outmethod to corresponding
	QString getCurrentString(); // returns outstring

	static debugtrace* instanse();
	static debugtrace& getObject();
	static void init(DebugPriority priority, OutputMode mode,
		QVector<OutputMode> OutputTo = QVector<OutputMode>(), int maxMsgLen = 60, bool notMixing = true);

	static QString compressNetMsg(QString);
	static QString shiftRight(QString);
};
// This is main static object of this module. You should not create more
// instances of it, instead use macroses.

// this whitelist defines which streams to use

unsigned long long int makeMsgId();
