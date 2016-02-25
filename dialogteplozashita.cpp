#include "dialogteplozashita.h"
#include "fragment.h"
#include "ui_dialogteplozashita.h"

DialogTeploZashita::DialogTeploZashita(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTeploZashita)
{
    ui->setupUi(this);    

}

DialogTeploZashita::~DialogTeploZashita()
{
    delete ui;
    delete m_model;
}

void DialogTeploZashita::setModel(QSqlRelationalTableModel *model)
{
    m_model = model;
    ui->tableView->setModel(m_model);
}

QSqlRelationalTableModel *DialogTeploZashita::model() const
{
    return m_model;
}
