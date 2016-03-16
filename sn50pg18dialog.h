#ifndef SN50PG18DIALOG_H
#define SN50PG18DIALOG_H

#include <QDialog>

namespace Ui {
class sn50pg18dialog;
}

class SN50pg18dialog : public QDialog
{
    Q_OBJECT

public:
    explicit SN50pg18dialog(QWidget *parent = 0);
    ~SN50pg18dialog();

private:
    Ui::sn50pg18dialog *ui;
};

#endif // SN50PG18DIALOG_H
