#include "dialogteplozashita.h"
#include "fragment.h"
#include "ui_dialogteplozashita.h"

DialogTeploZashita::DialogTeploZashita(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTeploZashita)
{
    ui->setupUi(this);

    model = new QSqlRelationalTableModel();
    model->setTable(Fragment::EntityName);

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setRelation(0,QSqlRelation(Section::EntityName,Section::ID,Section::Name));

    model->setHeaderData(0,Qt::Horizontal,QString("Наименование фрагмента"));
    model->setHeaderData(1,Qt::Horizontal,QString("tпом"));
    model->setHeaderData(2,Qt::Horizontal,QString("Aф,I, м2"));
	
	model->select();

    ui->tableView->setModel(model);
}

DialogTeploZashita::~DialogTeploZashita()
{
    delete ui;
    delete model;
}
