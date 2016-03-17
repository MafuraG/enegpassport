#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "buildingtype.h"
#include "energypassportmodel.h"
#include "sn50pg18dialog.h"
#include "t1t2dialog.h"
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

    void on_action_triggered();

    void on_action_5_triggered(bool checked);

    //void on_treeView_doubleClicked(const QModelIndex &index);

    void on_action_open_lookup_triggered();

    void on_action_sp_50_13330_2012_18_lookup_triggered();

private:
    Ui::MainWindow *ui;
    T1T2Dialog *t1t2_dialog;
    SN50pg18dialog *sn50pg18_dialog;
    BuildingType *btype;

    void resizeView(QAbstractItemView *view, const QString vtype, QAbstractItemModel *model);

    EnergyPassportModel *energyModel = nullptr ;
    QString dbname ;

    void loadStyleSheet(const QString &sheetName);
    void SwitchToFullTree(const bool on);
};

#endif // MAINWINDOW_H
