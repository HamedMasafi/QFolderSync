#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "syncer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Syncer s;
//    s.setFolderPath("/doc/dev/Qt/FtpSync");
//    s.setHost("ftp.giftbazaar.store");
//    s.setUsername("tooska");
//    s.setPassword("Ibmt93_6");
//    s.setPort(21);
//    s.start();

    MainWindow w;
    w.show();

    return a.exec();
}
