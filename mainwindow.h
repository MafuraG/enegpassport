#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "treemodel.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TreeModel *model;
};

#endif // MAINWINDOW_H
