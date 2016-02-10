#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"

#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QFile file(":/menu_structure.txt");
    file.open(QIODevice::ReadOnly);
    model = new TreeModel(file.readAll());
    file.close();

    ui->setupUi(this);

    ui->treeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}
