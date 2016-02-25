#ifndef DIALOGTEPLOZASHITA_H
#define DIALOGTEPLOZASHITA_H

#include <QDialog>
#include <QSqlRelationalTableModel>

namespace Ui {
class DialogTeploZashita;
}

class DialogTeploZashita : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTeploZashita(QWidget *parent = 0);
    ~DialogTeploZashita();

    void setModel(QSqlRelationalTableModel *model);

private:
    Ui::DialogTeploZashita *ui;
    QSqlRelationalTableModel *m_model;
};

#endif // DIALOGTEPLOZASHITA_H
