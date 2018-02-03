#ifndef SYNCER_H
#define SYNCER_H

#include <QObject>
#include <QString>

#include <3rdparty/qftp/qurlinfo.h>

class QFtp;
class FolderInfo;
class QBuffer;
class Syncer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dataFileName READ dataFileName WRITE setDataFileName NOTIFY dataFileNameChanged)
    Q_PROPERTY(QString folderPath READ folderPath WRITE setFolderPath NOTIFY folderPathChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)

    QString m_dataFileName;
    QString m_folderPath;
    QString m_host;
    QString m_username;
    QString m_password;
    quint16 m_port;
    QFtp *ftp;
    QBuffer *remoteHashBuffer;
    bool listFoundOnServer;
    FolderInfo *remoteList;
    FolderInfo *localList;

    void downloadList();
    void compair();


public:
    Syncer(QObject *parent = Q_NULLPTR);
    ~Syncer();

    void start();

    QString dataFileName() const;
    QString folderPath() const;
    QString host() const;
    QString username() const;
    QString password() const;
    quint16 port() const;


public slots:
    void setDataFileName(QString dataFileName);
    void setFolderPath(QString folderPath);
    void setHost(QString host);
    void setUsername(QString username);
    void setPassword(QString password);
    void setPort(quint16 port);

    void on_ftp_stateChanged(int);
    void on_ftp_listInfo(const QUrlInfo&);
    void on_ftp_dataTransferProgress(qint64, qint64);
    void on_ftp_rawCommandReply(int, const QString&);
    void on_ftp_commandStarted(int);
    void on_ftp_commandFinished(int, bool);
    void on_ftp_done(bool);

signals:
    void status(const QString &status);

    void dataFileNameChanged(QString dataFileName);
    void folderPathChanged(QString folderPath);
    void hostChanged(QString host);
    void usernameChanged(QString username);
    void passwordChanged(QString password);
    void portChanged(quint16 port);
};

#endif // SYNCER_H
