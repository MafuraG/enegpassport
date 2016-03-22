#ifndef BUILDINGTYPE_H
#define BUILDINGTYPE_H

#include "energypassportmodel.h"

#include <QDialog>

namespace Ui {
class BuildingType;
}

class BuildingType : public QDialog
{
    Q_OBJECT

public:
    explicit BuildingType(QWidget *parent = 0);
    ~BuildingType();

    EnergyPassportModel::TipZdaniya TipZdaniya() const;
    void setTipZdaniya(const EnergyPassportModel::TipZdaniya &TipZdaniya);
signals:
    run_calculations(EnergyPassportModel::TipZdaniya tzdaniya);

private slots:
    void on_radioButton_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_6_clicked();

    void on_BuildingType_finished(int result);

private:
    Ui::BuildingType *ui;
    EnergyPassportModel::TipZdaniya m_TipZdaniya ;
};

#endif // BUILDINGTYPE_H
