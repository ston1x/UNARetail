#pragma once
#include <QObject>





class QNetworkAccessManager;
class QNetworkReply;
class QTimer;
class AsyncRequestEngine : public QObject
{
	Q_OBJECT
protected:
	QNetworkReply* awaitedReply;
	QTimer* timeoutTimer;
	QString coreLink;
	bool repeatable;
	bool _setReply(QNetworkReply*);
public:
	AsyncRequestEngine(QString CoreLink, bool repeat, QObject* parent = Q_NULLPTR);
	void clearAndRequest(QString linkTail);
	void clear();
	void reinit(QString CoreLink, int timeoutInterval);

protected slots:
	void responseArrived();
	void errorArrived();
	void timeout();
signals:
	void clearResponse(QString);
	void parsedResponse(QStringList);
	void errorResponse(int);
};