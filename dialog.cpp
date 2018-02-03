#include "dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
