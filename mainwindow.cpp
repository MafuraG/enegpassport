#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"

#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList headers;
       headers << tr("Показатель") << tr("Обозначение показателя и единицы измерения")<< tr("Нормативное значение показателя")<< tr("Расчетное (проектное) значение показателя")<<tr("Фактическое значение показателя");

    QFile file(":/menu_structure.txt");
    file.open(QIODevice::ReadOnly);
    model = new TreeModel(headers,file.readAll());
    file.close();

    ui->treeView->setModel(model);

    for(int column = 0; column< model->columnCount(); ++column){
        ui->treeView->resizeColumnToContents(column);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
