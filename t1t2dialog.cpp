#include "t1t2dialog.h"
#include "ui_t1t2dialog.h"

T1T2Dialog::T1T2Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T1T2Dialog)
{
    ui->setupUi(this);
}

T1T2Dialog::~T1T2Dialog()
{
    delete ui;
}
