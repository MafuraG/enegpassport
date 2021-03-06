#include "energypassportmodel.h"
#include "mainwindow.h"
#include "treeitem.h"
#include "treemodel.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <typeinfo>
#include <QDebug>

#include <QFile>
#include <xlsxdocument.h>

QString MainWindow::DB_NAME = "enegdb.sqlite";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    t1t2_dialog = new T1T2Dialog();
    sn50pg18_dialog = new SN50pg18dialog();
    btype = new BuildingType();    

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        QMessageBox::critical(this, "Программа не запускается", "Необходимо установить SQLITE drivers");
        this->close();
    }

    //open model database
    //STRICTLY for testing on Production I will use current directory with exe
//    dbname = "C:/qt_projects/enegpassport/enegdb.sqlite";
//    dbname = "C:/Users/MafuraG/Documents/GitHub/enegpassport/enegdb.sqlite";
    dbname = QString("%0/%1").arg(qApp->applicationDirPath(),MainWindow::DB_NAME);

    //qDebug()<<dbname;

//    if  (energyModel != nullptr){
//        //save current energy model then close db
//        delete energyModel;
//        energyModel = nullptr;
//    }

    energyModel = new EnergyPassportModel(dbname);

    ui->treeView->setModel(energyModel->treeModel());
//    ui->tableView->setModel(energyModel->pakazatelModel());
    ui->tableView_2->setModel(energyModel->fragmentModel());

    //resizeView(ui->treeView,typeid(ui->treeView).name(),energyModel->treeModel());
//    resizeView(ui->tableView,typeid(ui->tableView).name(),energyModel->pakazatelModel());
    resizeView(ui->tableView_2,typeid(ui->tableView_2).name(),energyModel->fragmentModel());

//    ui->tableView->setColumnHidden(0,true);
//    ui->tableView->setColumnHidden(3,true);
//    ui->tableView->setColumnHidden(4,true);
//    ui->tableView->setColumnHidden(5,true);    

    SwitchToFullTree(false);

    //loadStyleSheet("custom");

    //connections
    connect(btype,&BuildingType::run_calculations,this,&MainWindow::run_calculations);
    //connect(ui->tableView_2, &QTableView::doubleClicked, this, &MainWindow::on_tableView_2_doubleClicked);

}

void MainWindow::SwitchToFullTree(const bool on){
    QTreeView *tv = ui->treeView;
    tv->setColumnWidth(0, 500);
    tv->setColumnWidth(1, 200);
    tv->setColumnWidth(2, 150);
    tv->setColumnWidth(3, 150);
    if (on){
        //show all columns
        ui->treeView->setColumnHidden(4,false);
        ui->treeView->setColumnHidden(5,false);
        ui->treeView->setColumnHidden(6,false);
        ui->treeView->setColumnHidden(7,false);

    }else
    {
        //Hide some columns in tree view
        ui->treeView->setColumnHidden(4,true);
        ui->treeView->setColumnHidden(5,true);
        ui->treeView->setColumnHidden(6,true);
        ui->treeView->setColumnHidden(7,true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete energyModel;
    delete t1t2_dialog;
    delete sn50pg18_dialog;
}

void MainWindow::run_calculations(EnergyPassportModel::TipZdaniya tzdaniya)
{
    energyModel->setTzdaniya(tzdaniya);
    energyModel->raschetPakazateli();
    ui->statusBar->showMessage(tr("Расчеты завершины."),3000);
}



void MainWindow::on_action_2_triggered()
{
    //load Passport data
    QString selectedFilter;
    QString startdir;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Загрузка данных"),
                                startdir,
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
    btype->show();
}

void MainWindow::on_action_triggered()
{
    //save heirachy to database
    energyModel->saveTreeModeltoDB();
    ui->statusBar->showMessage(tr("Структура показателей сохранена."),3000);
}

void MainWindow::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());

    qApp->setStyleSheet(styleSheet);
}

void MainWindow::on_action_5_triggered(bool checked)
{
    SwitchToFullTree(checked);

    QXlsx::Document doc(":/EnergTemplate.xlsx");

    //qDebug()<<doc.sheetNames()<<endl;
}

//void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
//{
//    if (index.isValid()) {
//        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
//        TreeItem *p_item = item->parent();
//        if (item){
//            unsigned int id = item->data(7).toUInt();
//            unsigned int p_id = 0;
//            if (p_item != nullptr) p_id = p_item->data(7).toUInt();

//            if (p_id == 103 || id == 103 ||
//                    p_id == 106 || id == 106){

//            }

//        }
//    }
//}

void MainWindow::on_action_open_lookup_triggered()
{
    t1t2_dialog->show();
}

void MainWindow::on_action_sp_50_13330_2012_18_lookup_triggered()
{
    sn50pg18_dialog->show();
}

void MainWindow::on_action_export_calculation_triggered()
{
    //Export calcultion in xls file
    //Save model data to file
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                tr("Сохранить расчета в Excel"),
                                QDir::currentPath(),
                                tr("Файл Excel (*.xlsx)"),
                                &selectedFilter);
    if (!fileName.isEmpty()){
        QString xlsx_template = ":/EnergTemplate.xlsx";
        energyModel->writeXlsReport(xlsx_template,fileName);
        ui->statusBar->showMessage(tr("Файл сохранен."),3000);
    }
}

void MainWindow::on_fragAddBtn_clicked()
{
    //add fragment
    energyModel->newFragment();
}

void MainWindow::on_fragDeleteBtn_clicked()
{
    //remove fragment
    energyModel->removeFragment(choosen_row);
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    //choose fragment
    if (index.isValid()) choosen_row = index.row();
}
