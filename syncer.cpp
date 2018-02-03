#include "syncer.h"

#include "3rdparty/qftp/qftp.h"
#include "folderchecksum.h"
#include <QDebug>

#include <QBuffer>
#include <QThread>

void Syncer::downloadList()
{
    if (listFoundOnServer) {
        remoteHashBuffer = new QBuffer(this);
        remoteHashBuffer->open(QBuffer::ReadWrite);
        ftp->get(m_dataFileName, remoteHashBuffer);
    } else {
        remoteList = new FolderInfo;
        compair();
    }
}

void Syncer::compair()
{
    QSet<QString> filesToUpload;
    QSet<QString> filesToRemoteDelete;

    localList = FolderInfo::fromDir(m_folderPath);
    qDebug() << "compair";
    foreach (auto f, localList->keys()) {
        auto localInfo = localList->value(f);
        auto remoteInfo = remoteList->value(f);

        if (localInfo.hash != remoteInfo.hash) {
            filesToUpload.insert(f);
        }
    }
    foreach (auto f, remoteList->keys()) {
        auto localInfo = localList->value(f);
        auto remoteInfo = remoteList->value(f);

        if (localInfo.hash == QString()) {
            filesToRemoteDelete.insert(f);
        }
    }

    qDebug() << "Uplaod"<<filesToUpload;
    qDebug() << "Delete"<<filesToRemoteDelete;
    ftp->put(localList->toByteArray(), m_dataFileName);
}

Syncer::Syncer(QObject *parent) : QObject(parent), m_dataFileName("FolderSync.dat")
{
    ftp = new QFtp(this);
    ftp->setObjectName("ftp");
    QMetaObject::connectSlotsByName(this);
}

Syncer::~Syncer()
{
    ftp->abort();
}

void Syncer::start()
{
    localList = FolderInfo::fromDir(m_folderPath);
    listFoundOnServer = false;
    int ok;
    emit status("Connecting to host");
    ftp->connectToHost(m_host);
    ftp->login(m_username, m_password);
    ftp->list();
}

// Properties
QString Syncer::dataFileName() const
{
    return m_dataFileName;
}

QString Syncer::folderPath() const
{
    return m_folderPath;
}

QString Syncer::host() const
{
    return m_host;
}

QString Syncer::username() const
{
    return m_username;
}

QString Syncer::password() const
{
    return m_password;
}

quint16 Syncer::port() const
{
    return m_port;
}

void Syncer::setDataFileName(QString dataFileName)
{
    if (m_dataFileName == dataFileName)
        return;

    m_dataFileName = dataFileName;
    emit dataFileNameChanged(m_dataFileName);
}

void Syncer::setFolderPath(QString folderPath)
{
    if (m_folderPath == folderPath)
        return;

    m_folderPath = folderPath;
    emit folderPathChanged(m_folderPath);
}

void Syncer::setHost(QString host)
{
    if (m_host == host)
        return;

    m_host = host;
    emit hostChanged(m_host);
}

void Syncer::setUsername(QString username)
{
    if (m_username == username)
        return;

    m_username = username;
    emit usernameChanged(m_username);
}

void Syncer::setPassword(QString password)
{
    if (m_password == password)
        return;

    m_password = password;
    emit passwordChanged(m_password);
}

void Syncer::setPort(quint16 port)
{
    if (m_port == port)
        return;

    m_port = port;
    emit portChanged(m_port);
}

//ftp events
void Syncer::on_ftp_stateChanged(int n)
{
}

void Syncer::on_ftp_listInfo(const QUrlInfo &url)
{
    if (url.name() == m_dataFileName) {
        listFoundOnServer = true;
        qDebug() << "found";
    }
    qDebug() << url.name();
}

void Syncer::on_ftp_dataTransferProgress(qint64, qint64)
{

}

void Syncer::on_ftp_rawCommandReply(int, const QString &)
{

}

void Syncer::on_ftp_commandStarted(int)
{
    qDebug() << "Command started" ;
}

void Syncer::on_ftp_commandFinished(int id, bool ok)
{
    switch (ftp->currentCommand()) {
    case QFtp::List:
        qDebug() << "List";
        downloadList();
        break;

    case QFtp::Put:
        qDebug() << m_dataFileName << "Uploaded to server" << ok;
        break;

    case QFtp::Get:
        remoteList = FolderInfo::fromJson(remoteHashBuffer->buffer());
        qDebug() << "remote hashes read"<<remoteHashBuffer->buffer();

        compair();
    }
}

void Syncer::on_ftp_done(bool)
{

}
