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
       headers << tr("Title") << tr("Description");

    QFile file(":/menu_structure.txt");
    file.open(QIODevice::ReadOnly);
    model = new TreeModel(headers,file.readAll());
    file.close();

    ui->treeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}
