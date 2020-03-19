#include "SearchDatabaseSettings.h"
#include <QTimer>
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "dataproviders/sqldataprovider.h"
#include <QMessageBox>


SearchDatabaseSettings::SearchDatabaseSettings(QWidget* parent)
	: QWidget(parent),mainLayout(new QFormLayout(this)), 
	httpDownloadUrl(new QLineEdit(this)),
	httpUploadUrl(new QLineEdit(this)),
	downloadNow(new MegaIconButton(this)),
	downloadProgress(new QProgressBar(this)),
	downloadcenter(Modes::Search),
	databasePath(new QLineEdit(this)),
	fromFileLoad(new MegaIconButton(this)),
	timeoutTimer(new QTimer(this))
{
	mainLayout->addRow(tr("Remote database url"), httpDownloadUrl);
	mainLayout->addRow(tr("Remote receiver url"), httpUploadUrl);
	mainLayout->addWidget(downloadProgress);
	mainLayout->addWidget(downloadNow);
	mainLayout->addRow(tr("local database path"), databasePath);
	mainLayout->addWidget(fromFileLoad);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->setLabelAlignment(Qt::AlignCenter);
	mainLayout->setFormAlignment(Qt::AlignCenter);
	mainLayout->setRowWrapPolicy(QFormLayout::RowWrapPolicy::WrapAllRows);

	downloadNow->setText(tr("Download"));
	downloadNow->setIcon(QIcon(":/res/netDownload.png"));
	downloadNow->setStyleSheet(DOWNLOAD_BUTTONS_STYLESHEET);

	fromFileLoad->setText(tr("Load from local"));
	fromFileLoad->setIcon(QIcon(":/res/viaFile.png"));
	fromFileLoad->setStyleSheet(DOWNLOAD_BUTTONS_STYLESHEET);

	httpDownloadUrl->setText(AppSettings->httpIn.toString());
	httpUploadUrl->setText(AppSettings->httpOut.toString());

	timeoutTimer->setInterval(20000);
	timeoutTimer->setSingleShot(true);
	QObject::connect(httpDownloadUrl, &QLineEdit::returnPressed, httpUploadUrl,
		QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(httpUploadUrl, &QLineEdit::returnPressed, fromFileLoad,
		QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(downloadNow, &QPushButton::clicked, this, &SearchDatabaseSettings::downloadDatabase);
	QObject::connect(&downloadcenter, &toHttp::progressLeap, this, &SearchDatabaseSettings::downloadProcess);
	QObject::connect(timeoutTimer, &QTimer::timeout, this, &SearchDatabaseSettings::downloadTimeout);
	QObject::connect(fromFileLoad, &MegaIconButton::clicked, this, &SearchDatabaseSettings::loadDatabaseFromFile);
}

void SearchDatabaseSettings::extractAndSave()
{
	AppSettings->httpIn = QUrl::fromUserInput(httpDownloadUrl->text());
	AppSettings->httpOut = QUrl::fromUserInput(httpUploadUrl->text());
	AppSettings->localDatabase = databasePath->text();
}

void SearchDatabaseSettings::retranslate()
{
	qobject_cast<QLabel*>(mainLayout->labelForField(httpDownloadUrl))->setText(tr("Remote database url"));
	qobject_cast<QLabel*>(mainLayout->labelForField(httpUploadUrl))->setText(tr("Remote receiver url"));
	qobject_cast<QLabel*>(mainLayout->labelForField(databasePath))->setText(tr("local database path"));
	downloadNow->setText(tr("Download"));
	fromFileLoad->setText(tr("Load from local"));
}

void SearchDatabaseSettings::downloadDatabase()
{
	if (downloadcenter.isUsed())
		return;
	downloadProgress->setValue(1);
	downloadNow->setDisabled(true);
	timeoutTimer->start();
	downloadcenter.addressChanged(httpDownloadUrl->text() + "?c=download", true);
	downloadcenter.get();
}

void SearchDatabaseSettings::loadDatabaseFromFile()
{
	if (databasePath->text().isEmpty())
	{
		return;
	}
	else
	{
		QFile f(databasePath->text());
		f.open(QIODevice::ReadOnly);
		if (!f.isOpen() || !f.isReadable())
			return;

		QTextStream in(&f);
		ShortBarcodeEntity shb;
		in.readLine();
		while (!in.atEnd())
		{
			QString line = in.readLine();
			auto splitted = line.split(',');
			if (splitted.count() != 4)
			{
				f.close();
				QMessageBox::critical(this, tr("Error reading file"), tr("Error reading file"));
				return;
			}
			shb.barcode = splitted.at(0);
			shb.code = splitted.at(1).toInt();
			shb.count = splitted.at(2);
			shb.info = splitted.at(3);
			AppData->pushIntoDownloaded(shb);
		}
		QMessageBox::warning(this, tr("Success"), tr("File read succesfully"));
		f.close();
	}
}

void SearchDatabaseSettings::downloadTimeout()
{
	downloadcenter.dropAwaiting();
	downloadNow->setDisabled(false);
}

void SearchDatabaseSettings::downloadProcess(int proc)
{
	downloadProgress->setValue(proc);
	timeoutTimer->stop();
	if (proc == 100)
	{
		downloadNow->setDisabled(false);
	}
}
