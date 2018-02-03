#ifndef FOLDERCHECKSUM_H
#define FOLDERCHECKSUM_H

#include <QString>
#include <QMap>

struct FileInfo
{
public:
    quint32 size;
    QString hash;
};

class FolderInfo : public QMap<QString, FileInfo>
{
    QString _path;
public:
    FolderInfo();

    static FolderInfo *fromDir(const QString &path);
    static FolderInfo *fromJson(const QByteArray &array);
    QByteArray toByteArray();

private:
    QString removePath(const QString &path);
    void checkFolder(const QString &p);
    void checkFile(const QString &p);
};

class FolderCheckSum
{
    QString _path;
    QMap<QString, QString> _checkSums;
    QList<QString> _files;
public:
    FolderCheckSum();
    FolderCheckSum(QString path);
    void calc();
    QMap<QString, QString> checkSums() const;
    QList<QString> files() const;
    QByteArray toByteArray();

    static FolderCheckSum *fromDir(const QString &path);
    static FolderCheckSum *fromByteArray(const QByteArray &array);
private:
    QString removePath(const QString &path);
    void checkFolder(const QString &p);
    QByteArray fileChecksum(const QString &fileName);
};

#endif // FOLDERCHECKSUM_H
