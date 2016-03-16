#include "sn50pg18dialog.h"
#include "ui_sn50pg18dialog.h"

SN50pg18dialog::SN50pg18dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sn50pg18dialog)
{
    ui->setupUi(this);
}

SN50pg18dialog::~SN50pg18dialog()
{
    delete ui;
}
