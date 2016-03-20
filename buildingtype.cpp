#include "buildingtype.h"
#include "ui_buildingtype.h"

BuildingType::BuildingType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuildingType)
{
    ui->setupUi(this);
}

BuildingType::~BuildingType()
{
    delete ui;
}
EnergyPassportModel::TipZdaniya BuildingType::TipZdaniya() const
{
    return m_TipZdaniya;
}

void BuildingType::setTipZdaniya(const EnergyPassportModel::TipZdaniya &TipZdaniya)
{
    m_TipZdaniya = TipZdaniya;
}




void BuildingType::on_radioButton_clicked()
{
    //type 1
    setTipZdaniya(EnergyPassportModel::type1);
}

void BuildingType::on_radioButton_3_clicked()
{
    //type 2
    setTipZdaniya(EnergyPassportModel::type2);
}

void BuildingType::on_radioButton_2_clicked()
{
    //type 3
    setTipZdaniya(EnergyPassportModel::type3);
}

void BuildingType::on_radioButton_4_clicked()
{
    //type 4
    setTipZdaniya(EnergyPassportModel::type4);
}

void BuildingType::on_radioButton_5_clicked()
{
    setTipZdaniya(EnergyPassportModel::type5);
}

void BuildingType::on_radioButton_6_clicked()
{
    setTipZdaniya(EnergyPassportModel::type6);
}
