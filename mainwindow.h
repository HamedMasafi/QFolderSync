#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class Syncer;
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    Syncer *syncer;

public:
    explicit MainWindow(QWidget *parent = 0);

public slots:
    void on_syncer_log(const QString &log);

protected:
    void changeEvent(QEvent *e);
};

#endif // MAINWINDOW_H
