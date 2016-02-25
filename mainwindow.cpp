#include "energypassportmodel.h"
#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

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

    energyModel = nullptr;

    //eneryModel = new EnergyPassportModel("");

    //ui->treeView->setModel(model);


//    for(int column = 0; column< model->columnCount(); ++column){
//        ui->treeView->resizeColumnToContents(column);
//    }
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
    //open model database
    dbname = QFileDialog::getOpenFileName(this,"Выбор файла");
    if  (energyModel != nullptr){
        //save current energy model then close db
        delete energyModel;
        energyModel = nullptr;
    }

    energyModel = new EnergyPassportModel(dbname);

    ui->treeView->setModel(energyModel->treeModel());
    ui->tableView->setModel(energyModel->pakazatelModel());
    ui->tableView_2->setModel(energyModel->fragmentModel());

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
}
