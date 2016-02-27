#include "energypassportmodel.h"
#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <typeinfo>
#include <QDebug>

#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        QMessageBox::critical(this, "Программа не запускается", "Необходимо установить SQLITE drivers");
        this->close();
    }

    //open model database
    //dbname = QFileDialog::getOpenFileName(this,"Выбор файла");
    dbname = "C:/Users/MafuraG/Documents/GitHub/enegpassport/enegdb.sqlite";
//    if  (energyModel != nullptr){
//        //save current energy model then close db
//        delete energyModel;
//        energyModel = nullptr;
//    }

    energyModel = new EnergyPassportModel(dbname);

    ui->treeView->setModel(energyModel->treeModel());
    ui->tableView->setModel(energyModel->pakazatelModel());
    ui->tableView_2->setModel(energyModel->fragmentModel());

    resizeView(ui->treeView,typeid(ui->treeView).name(),energyModel->treeModel());
    resizeView(ui->tableView,typeid(ui->tableView).name(),energyModel->pakazatelModel());
    resizeView(ui->tableView_2,typeid(ui->tableView_2).name(),energyModel->fragmentModel());

}

MainWindow::~MainWindow()
{
    delete ui;
    delete energyModel;
}

void MainWindow::on_action_triggered()
{
    //new model
}

void MainWindow::on_action_2_triggered()
{

}



void MainWindow::on_action_3_triggered()
{
    //close application
    if  (energyModel != nullptr){
        //save current energy model then close db
        delete energyModel;
        energyModel = nullptr;
    }

    this->close();
}

void MainWindow::on_action_5_triggered()
{
    //open fragments editor
    //energyModel->saveTreeModeltoDB();
}

void MainWindow::resizeView(QAbstractItemView *view,const QString vtype, QAbstractItemModel *model)
{
    for(int i = 0; i < model->columnCount(); i++){
//        qDebug()<< "type id view " << typeid(view).name();
//        qDebug()<< "type id QTreeView " << typeid(QTreeView*).name();
        if (vtype == typeid(QTreeView*).name())
            ((QTreeView*)view)->resizeColumnToContents(i);
        if (vtype == typeid(QTableView*).name())
            ((QTableView*)view)->resizeColumnToContents(i);
    }
}


