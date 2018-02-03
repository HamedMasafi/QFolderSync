#include "folderchecksum.h"
#include "mainwindow.h"

#include "syncer.h"
#include <QFileSystemModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    syncer = new Syncer(this);
    syncer->setObjectName("syncer");

    setupUi(this);

    QFileSystemModel *model = new QFileSystemModel;
    model->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    model->setRootPath(qApp->applicationDirPath());
    treeView->setModel(model);

    QModelIndex indx = model->index("/");
    qDebug() << indx.data();

    auto f = FolderInfo::fromDir("/doc/dev/Qt/FtpSync");
}

void MainWindow::on_syncer_log(const QString &log)
{
    textBrowser->append(log);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
