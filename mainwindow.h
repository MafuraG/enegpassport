#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "energypassportmodel.h"
#include "treemodel.h"

#include <QAbstractItemView>
#include <QAbstractItemModel>
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

private slots:    

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_6_triggered();

    void on_action_4_triggered();

private:
    Ui::MainWindow *ui;
    void resizeView(QAbstractItemView *view, const QString vtype, QAbstractItemModel *model);

    EnergyPassportModel *energyModel = nullptr ;
    QString dbname ;
};

#endif // MAINWINDOW_H
