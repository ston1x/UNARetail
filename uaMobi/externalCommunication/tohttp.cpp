#include "tohttp.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif


toHttp::toHttp(Modes mode)
	:communicationCore(mode), inProgress(false)
{
	connect(this, SIGNAL(readNL()), this, SLOT(readingNL()), Qt::QueuedConnection);
}

void toHttp::get()
{
	if (inProgress)
	{
		return;
	}
	inProgress = true;
	if (!checkFromAdress())
	{
		emit addressInvalid();
		return;
	}

	QNetworkRequest req;
	req.setUrl(fromAdress);

	manager = new QNetworkAccessManager();

	bool connstate;
	connstate = connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadResponce(QNetworkReply*)));
	manager->get(req);
}

void toHttp::dropAwaiting()
{
	inProgress = false;
}

void toHttp::downloadResponce(QNetworkReply* replay)
// This slot is receiving data blocks
{
	gReplay = replay;
	networkManager = sender();

	if (replay->error() != 0)
	{


		emit operationDone(false);
	}
	else
	{
		if (gReplay->size() != 0)
		{

			li = gReplay->size();
			lip = 0;


			AppData->recreateDownloadTable();
			input = "("; // prepared table and input string for downloading
		}

		emit readNL();
	}
}

void toHttp::readingNL()
// This slot is reading lines from data block, deleting rn's, then adds this to input line
{


	if (!gReplay->atEnd())
	{


		QString line = QString::fromUtf8(gReplay->readLine()).replace("\r", "").replace("\n", "),(");
		input += line;
		lip += line.size() + 1;

		emit progressLeap(lip * 100 / li);


		emit readNL();
	}
	else
	{


		input = input.left(input.length() - 2);

		AppData->downloadFullList(input);
		emit operationDone(true);
		gReplay->deleteLater();
		networkManager->deleteLater();
		inProgress = false;
	}
}

bool toHttp::send(sendingMode mode, int fileFormat)
{

	loadDataToSend(currentmode, mode, fileFormat);

	if (!checkLoadData())
	{
		emit dataErrorOccured();
		return false;
	};

	if (!checkToAdress())
	{
		emit addressInvalid();
		return false;
	};

#ifdef DEBUG
	detrace_OCREATED("QByteArray", "toHttp::send");
#endif

	QByteArray boundary;
	// forming upload list. Do not touch

#ifdef DEBUG
	detrace_OCREATED("QByteArray", "toHttp::send");
#endif

	QByteArray datas(QString("--" + boundary + "\r\n").toLatin1());
	datas += "Content-Disposition: multipart/form-data; name=\"file\"; filename=\"" + deviceName + "\"\r\n";
	datas += "Content-Type: multipart/form-data\r\n\r\n";
	datas += uploadList;
	datas += "\r\n";
	datas += QString("--" + boundary + "\r\n\r\n").toLatin1();
	datas += "Content-Disposition: form-data; name=\"upload\"\r\n\r\n";
	datas += "Uploader\r\n";
	datas += QString("--" + boundary + "--\r\n").toLatin1();

#ifdef DEBUG

	detrace_OCREATED("QNetworkRequest", "tohttp::send");
	detrace_METHTEXTS("tohttp::send", "datas", QString::fromStdString(datas.toStdString()));

#endif
	QNetworkRequest req;
	req.setUrl(toAdress + "?c=upload");
	req.setRawHeader("Content-Type", "multipart/form-data; boundary=" + boundary);

#ifdef DEBUG
	detrace_OCREATED("QNetworkAccessmanager", "tohttp::send");
#endif
	manager = new QNetworkAccessManager();

	bool connstate;
	connstate = connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadResponce(QNetworkReply*)));

#ifdef DEBUG
	detrace_CONNECTSTAT("finished to uploadResponce", connstate);
	detrace_METHINVOK("post", "QNetworkAccessManager", "send", "tohttp");
	detrace_METHTEXTS("tohttp::send", "data", datas.data());
	detrace_METHEXPL("req: " << req.url().toString());
#endif
	manager->post(req, datas);
	return true;
}

void toHttp::uploadResponce(QNetworkReply* replay)
//	This slot is responding to upload reply. It clears everything and then emits operationDone
{
#ifdef DEBUG
	detrace_SLOTCALL("uploadResponce", "toHttp");
#endif

	if (replay->error() != 0)
	{
#ifdef DEBUG
		detrace_METHPERROR("uploadResponce", "replay error");
#endif

		emit operationDone(false);
	}
	else
	{
		emit operationDone(true);
	}

#ifdef DEBUG
	detrace_METHEXPL("replay and network manager delete later called");
#endif

	replay->deleteLater();
	QObject* networkManager = sender();
	networkManager->deleteLater();
}