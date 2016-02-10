#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(enegresources);

    QApplication a(argc, argv);    

    MainWindow w;

    w.show();

    return a.exec();
}
