#ifndef T1T2DIALOG_H
#define T1T2DIALOG_H

#include <QDialog>

namespace Ui {
class T1T2Dialog;
}

class T1T2Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit T1T2Dialog(QWidget *parent = 0);
    ~T1T2Dialog();

private:
    Ui::T1T2Dialog *ui;
};

#endif // T1T2DIALOG_H
