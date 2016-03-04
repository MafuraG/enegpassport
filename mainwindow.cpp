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
    //STRICTLY for testing on Production I will use current directory with exe
    dbname = "C:/qt_projects/enegpassport/enegdb.sqlite";
//    dbname = "C:/Users/MafuraG/Documents/GitHub/enegpassport/enegdb.sqlite";
//    if  (energyModel != nullptr){
//        //save current energy model then close db
//        delete energyModel;
//        energyModel = nullptr;
//    }

    energyModel = new EnergyPassportModel(dbname);

    ui->treeView->setModel(energyModel->treeModel());
//    ui->tableView->setModel(energyModel->pakazatelModel());
    ui->tableView_2->setModel(energyModel->fragmentModel());

    resizeView(ui->treeView,typeid(ui->treeView).name(),energyModel->treeModel());
//    resizeView(ui->tableView,typeid(ui->tableView).name(),energyModel->pakazatelModel());
    resizeView(ui->tableView_2,typeid(ui->tableView_2).name(),energyModel->fragmentModel());

//    ui->tableView->setColumnHidden(0,true);
//    ui->tableView->setColumnHidden(3,true);
//    ui->tableView->setColumnHidden(4,true);
//    ui->tableView->setColumnHidden(5,true);

    ui->tableView_2->setColumnHidden(0,true);

    //Hide some columns in tree view
    ui->treeView->setColumnHidden(4,true);
    ui->treeView->setColumnHidden(5,true);
    ui->treeView->setColumnHidden(6,true);
    ui->treeView->setColumnHidden(7,true);
    ui->treeView->setStyleSheet(TVStyleSheet);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete energyModel;
}



void MainWindow::on_action_2_triggered()
{
    //load Passport data
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Загрузка данных"),
                                QDir::currentPath(),
                                tr("CSV Files (*.csv)"),
                                &selectedFilter
                                );
    if (!fileName.isEmpty()){
        energyModel->loadModelDataFromFile(fileName);
        ui->statusBar->showMessage(tr("Загрузка завершена."),3000);
    }
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



void MainWindow::on_action_6_triggered()
{
    //Save model data to file
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                tr("Сохранить Энергетический паспорт"),
                                QDir::currentPath(),
                                tr("CSV Files (*.csv)"),
                                &selectedFilter);
    if (!fileName.isEmpty()){
        energyModel->saveModelDatatoFile(fileName);
        ui->statusBar->showMessage(tr("Файл сохранен."),3000);
    }

}

void MainWindow::on_action_4_triggered()
{
    energyModel->raschetPakazateli();
    ui->statusBar->showMessage(tr("Расчеты завершины."),3000);
}

void MainWindow::on_action_triggered()
{
    //save heirachy to database
    energyModel->saveTreeModeltoDB();
    ui->statusBar->showMessage(tr("Структура показателей сохранена."),3000);
}

QString MainWindow::TVStyleSheet = "QTreeView::{ " +
                                   "    alternate-background-color:#f6fafb" +
                                   "    background: #e8f4fc" +
                                   "}";
