#include "AsyncRequestEngine.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QLinkedList>
#include "communicationCore.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
bool AsyncRequestEngine::_setReply(QNetworkReply* reply)
{
	if (awaitedReply != Q_NULLPTR)
	{
		awaitedReply->disconnect();
		awaitedReply->deleteLater();
	}
	awaitedReply = reply;
	return reply != Q_NULLPTR;
}

AsyncRequestEngine::AsyncRequestEngine(QString CoreLink, bool repeat, QObject* parent)
	: QObject(parent), 
	awaitedReply(Q_NULLPTR), timeoutTimer(new QTimer(this)),
	coreLink(CoreLink), repeatable(repeat)
{
	timeoutTimer->setSingleShot(true);
	timeoutTimer->setInterval(communicationCore::instanse()->getTimeoutInterval());
	QObject::connect(timeoutTimer, &QTimer::timeout, this, &AsyncRequestEngine::timeout);
}

void AsyncRequestEngine::clearAndRequest(QString linkTail)
{
	if (!_setReply(communicationCore::sendGETRequest(coreLink + linkTail)))
		return;
	QObject::connect(awaitedReply, &QNetworkReply::finished, this, &AsyncRequestEngine::responseArrived);
	QObject::connect(awaitedReply, QOverload<>::of(&QNetworkReply::error), this, &AsyncRequestEngine::errorArrived);
}

void AsyncRequestEngine::clear()
{
	if (awaitedReply != Q_NULLPTR)
	{
		awaitedReply->disconnect();
		awaitedReply->deleteLater();
		awaitedReply = Q_NULLPTR;
	}
}

void AsyncRequestEngine::reinit(QString CoreLink, int timeoutInterval)
{
	coreLink = CoreLink;
	timeoutTimer->setInterval(timeoutInterval);
}

void AsyncRequestEngine::responseArrived()
{
	QString response = QString::fromUtf8(awaitedReply->readAll());
#ifdef DEBUG
	detrace_NETRESPARR(response, awaitedReply->url().toString(), "Async");
#endif
	if (!response.isEmpty())
	{
		emit clearResponse(response);
	}
	QStringList splitted = response.split(QChar(30));
	if (splitted.isEmpty())
		return;
	if (splitted.first().contains("200"))
	{
		if (splitted.count() >= 2)
		{
			QStringList parsed;
			parsed << splitted.at(1);
			emit parsedResponse(parsed);
		}
	}
	else
	{
#ifdef DEBUG
		detrace_NETERROR("no content", response);
#endif
	}
}

void AsyncRequestEngine::errorArrived()
{
	emit errorResponse(awaitedReply->error());
	if (repeatable)
	{
		if (!_setReply(communicationCore::sendGETRequest(awaitedReply->url())))
			return;
		QObject::connect(awaitedReply, &QNetworkReply::finished, this, &AsyncRequestEngine::responseArrived);
		QObject::connect(awaitedReply, QOverload<>::of(&QNetworkReply::error), this, &AsyncRequestEngine::errorArrived);
		return;
	}
	clear();
}

void AsyncRequestEngine::timeout()
{
	emit errorResponse(-1);
	if (repeatable)
	{
		if (!_setReply(communicationCore::sendGETRequest(awaitedReply->url())))
			return;
		QObject::connect(awaitedReply, &QNetworkReply::finished, this, &AsyncRequestEngine::responseArrived);
		QObject::connect(awaitedReply, QOverload<>::of(&QNetworkReply::error), this, &AsyncRequestEngine::errorArrived);
		return;
	}
	clear();
}
