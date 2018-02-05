#ifndef FILESMODEL_H
#define FILESMODEL_H

#include <QAbstractItemModel>

class FilesModel : public QAbstractItemModel
{
public:
    FilesModel(QObject *parent = Q_NULLPTR);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // FILESMODEL_H
