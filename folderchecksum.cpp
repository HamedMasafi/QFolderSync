#include "folderchecksum.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QMap<QString, QString> FolderCheckSum::checkSums() const
{
    return _checkSums;
}

QString FolderCheckSum::removePath(const QString &path)
{
    QString ret = path;
    if (ret.startsWith(_path))
        ret = ret.remove(0, _path.length());
    if (ret.startsWith("/"))
        ret = ret.remove(0, 1);
    return ret;
}

QList<QString> FolderCheckSum::files() const
{
    return _files;
}

QByteArray FolderCheckSum::toByteArray()
{
    QJsonDocument doc;
    QJsonArray filesArray;
    foreach (QString f, _files)
        filesArray.append(f);
    QJsonObject hashObject;
    QMapIterator<QString, QString> i(_checkSums);
    while (i.hasNext()) {
        i.next();
        hashObject.insert(i.key(), QJsonValue(i.value()));
    }
    QJsonObject mainObject;
    mainObject.insert("files", filesArray);
    mainObject.insert("hashes", hashObject);
    doc.setObject(mainObject);
    return doc.toJson();
}

FolderCheckSum *FolderCheckSum::fromDir(const QString &path)
{
    FolderCheckSum *r = new FolderCheckSum;
    r->checkFolder(path);
    return r;
}

FolderCheckSum *FolderCheckSum::fromByteArray(const QByteArray &array)
{
    FolderCheckSum *r = new FolderCheckSum;
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonArray filesArray = doc.object().value("files").toArray();
    foreach (QJsonValue v, filesArray)
        r->_files.append(v.toString());
    QJsonObject hashObject = doc.object().value("hashes").toObject();
    foreach (QString key, hashObject.keys())
        r->_checkSums.insert(key, hashObject[key].toString());
    return r;
}

FolderCheckSum::FolderCheckSum()
{

}

FolderCheckSum::FolderCheckSum(QString path)
{
    _path = path;
    if (!_path.endsWith("/"))
        _path.append("/");
}


void FolderCheckSum::calc()
{
    checkFolder(_path);
}

void FolderCheckSum::checkFolder(const QString &p)
{
    qDebug() << "Checking dir" << p;
    QDir dir(p);
    QStringList files = dir.entryList(QDir::Files);
    foreach (QString f, files)
        fileChecksum(p + "/" + f);

    QStringList dirs = dir.entryList(QDir::Dirs);
    foreach (QString d, dirs)
        if (d != "." && d != "..")
            checkFolder(p + "/" + d);
}

QByteArray FolderCheckSum::fileChecksum(const QString &fileName)
{
    qDebug() << "Checking file" << fileName;
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha224);
        if (hash.addData(&f)) {
            _files.append(fileName);
            _checkSums[removePath(fileName)] = hash.result();
            return hash.result();
        }
    }
    qDebug() << "  * NO HASH";
    return QByteArray();
}

FolderInfo::FolderInfo()
{

}

FolderInfo *FolderInfo::fromDir(const QString &path)
{
    FolderInfo *ret = new FolderInfo;
    ret->checkFolder(path);
    return ret;
}

FolderInfo *FolderInfo::fromJson(const QByteArray &array)
{
    qDebug() << "reading from json"<<array;
    FolderInfo *r = new FolderInfo;
    QJsonDocument doc = QJsonDocument::fromJson(array);

    QJsonObject o = doc.object();
    foreach (QString key, o.keys()) {
        FileInfo fi;
        QJsonObject v = o.value(key).toObject();
        fi.size = v.value("size").toInt();
        fi.hash = v.value("hash").toString();
        r->insert(key, fi);
    }
    return r;
}

QByteArray FolderInfo::toByteArray()
{
    QJsonDocument doc;
    QJsonObject mainObject;

    const_iterator i;
    for(i = begin(); i != end(); ++i) {
        QJsonObject obj
        {
            {"size", QJsonValue::fromVariant((*i).size)},
            {"hash", (*i).hash}
        };
        mainObject.insert(i.key(), obj);
    }
    doc.setObject(mainObject);
    return doc.toJson();
}

QString FolderInfo::removePath(const QString &path)
{
    QString ret = path;
    if (ret.startsWith(_path))
        ret = ret.remove(0, _path.length());
    if (ret.startsWith("/"))
        ret = ret.remove(0, 1);
    return ret;
}

void FolderInfo::checkFolder(const QString &p)
{
    qDebug() << "Checking dir" << p;
    QDir dir(p);
    QStringList files = dir.entryList(QDir::Files);
    foreach (QString f, files)
        checkFile(p + "/" + f);

    QStringList dirs = dir.entryList(QDir::Dirs);
    foreach (QString d, dirs)
        if (d != "." && d != "..")
            checkFolder(p + "/" + d);
}

void FolderInfo::checkFile(const QString &p)
{
    FileInfo fi;// = new FileInfo;
    QFile f(p);
    if (f.open(QFile::ReadOnly)) {
        fi.size = f.size();
//        fi.fullPath = p;
        QCryptographicHash hash(QCryptographicHash::Sha224);
        if (hash.addData(&f)) {
            fi.hash = QString(hash.result());
            this->insert(removePath(p), fi);
        }
    }
}
