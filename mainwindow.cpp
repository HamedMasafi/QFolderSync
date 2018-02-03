#include "folderchecksum.h"
#include "mainwindow.h"

#include "syncer.h"
#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath("/doc/dev/Qt/FtpSync");
    treeView->setModel(model);

    auto f = FolderInfo::fromDir("/doc/dev/Qt/FtpSync");

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
