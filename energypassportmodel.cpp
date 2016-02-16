#include "energypassportmodel.h"

EnergyPassportModel::EnergyPassportModel()
{

}

EnergyPassportModel::~EnergyPassportModel()
{

}

double EnergyPassportModel::compactnessFactor()
{
    if (model == null) return 0;

    Indicator *p_area = model->getIndicatorByName(area_boundary);
    Indicator *p_volume = model->getIndicatorByName(volume_heated_space);

    if (p_area != null && p_volume != null && p_volume->calcValue() != 0){
        return p_area->calcValue() / p_volume->calcValue();
    }
    return 0;
}

double EnergyPassportModel::kratnostvozdukhobmen()
{
    if (model == null) return 0;
    Indicator * p_kol_kvartiryi = model->getIndicatorByName(kol_kvartir);
    Indicator * p_area_living_space = model->getIndicatorByName(area_living_space);
    Indicator * p_heated_volume = model->getIndicatorByName(volume_heated_space);
    Indicator * p_volume_reduction = model->getIndicatorByName(coeff_volume_reduction);
    Indicator * p_area_doors = model->getIndicatorByName(area_doors);
    Indicator * p_area_windows = model->getIndicatorByName(area_windows_balcony);
    Indicator * p_vysota_zdan = model->getIndicatorByName(vysota_zdaniya);
    Indicator * p_skopost_veter = model->getIndicatorByName(max_wind_velocity);

    double lvent1 = 30 * p_kol_kvartiryi->calcValue() ;
    double lvent2 = 0.35 * 3 * p_area_living_space->calcValue();
    double lvent = 0;
    double y_ext = 12.68; //TODO Constant?
    double y_int = 12.68; //TODO Constant?
    double v = p_skopost_veter->calcValue();
    double delta_P_okna = 0.28 * (y_ext - y_int) +  0.03 * y_ext* v*v;
    double delta_P_dver = 0.55*(y_ext - y_int) +  0.03 * y_ext* v*v;
    double delta_P = 10;
    double Gn_ok = 5.00 ; //From snip
    double Gn_dv = 7.00;
    double R_ok =  (1/Gn_ok) * (delta_P_dver) ;
    double R_dv = 0;

    if (lvent1 > lvent2) lvent = lvent1;
    else lvent = lvent2;



    double lpritok = 3 * p_area_living_space->calcValue();
    double n_chisov = 68; // TODO get from model or constant?






}

double EnergyPassportModel::udelnayateplozashita()
{


}

double EnergyPassportModel::udelnayaventilyatsii()
{

}

double EnergyPassportModel::udelnayatenlovyidelenie()
{

}

double EnergyPassportModel::udelnayatenlopostunleniesontse()
{

}
TreeModel *EnergyPassportModel::getModel() const
{
    return model;
}

void EnergyPassportModel::setModel(TreeModel *value)
{
    model = value;
}











