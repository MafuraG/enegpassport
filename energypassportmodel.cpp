#include "energypassportmodel.h"
#include "treeitem.h"
#include <QFile>

EnergyPassportModel::EnergyPassportModel()
{

}

EnergyPassportModel::EnergyPassportModel(const QString dbname)
{
    ctx = new Dbctx("QSQLITE",dbname);

    QStringList headers;
       headers << Pakazatel::D_Name
               << Pakazatel::D_Unit
               << Pakazatel::D_NomValue
               << Pakazatel::D_CalcValue
               << Pakazatel::D_FactValue;

    QFile file(":/menu_structure.txt");
    file.open(QIODevice::ReadOnly);
    m_treeModel = new TreeModel(headers,file.readAll());
    file.close();

    ctx->initFragmentModel();
    ctx->initPakazatelModel();
}

EnergyPassportModel::~EnergyPassportModel()
{
    delete ctx;
}

double EnergyPassportModel::kompaktnost()
{
    if (m_treeModel == nullptr) return 0;

    Pakazatel *p_area = m_treeModel->getIndicatorByName(area_boundary);
    Pakazatel *p_heated_volume = m_treeModel->getIndicatorByName(volume_heated_space);

    if (p_area != nullptr && p_heated_volume != nullptr && p_heated_volume->calcValue() != 0){
        return p_area->calcValue() / p_heated_volume->calcValue();
    }
    return 0;
}

double EnergyPassportModel::soprativlenieVozdukhProniknovenie(const double Gn, const double delta_P_x, const double delta_P)
{
    double R =  (1/Gn) * (pow((delta_P_x /delta_P),(2/3.0))) ;
    return R;
}

double EnergyPassportModel::paznostDavlenie(const double k, const double y_int, const double v, const double y_ext)
{
    double delta_P = k * (y_ext - y_int) +  0.03 * y_ext* v*v;

    return delta_P;
}

double EnergyPassportModel::kolichestvoinfiltrvozdukh(const double delta_P_dver, const double A_ok, const double R_dv, const double A_dv,
                                                      const double R_ok, const double delta_P_okna)
{
    double G_inf = (((A_ok/R_ok) * (pow((delta_P_okna/10.0),2/3.0))) + A_dv/R_dv) *(pow((delta_P_dver/10), 1/2.0));
    return G_inf;
}

double EnergyPassportModel::kratnostvozdukhobmen()
{
    if (m_treeModel == nullptr) return 0;

    Pakazatel * p_kol_kvartiryi = m_treeModel->getIndicatorByName(kol_kvartir);
    Pakazatel * p_area_living_space = m_treeModel->getIndicatorByName(area_living_space);
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByName(volume_heated_space);
    Pakazatel * p_volume_reduction = m_treeModel->getIndicatorByName(coeff_volume_reduction);
    Pakazatel * p_area_doors = m_treeModel->getIndicatorByName(area_doors);
    Pakazatel * p_area_windows = m_treeModel->getIndicatorByName(area_windows_balcony);
    Pakazatel * p_vysota_zdanya = m_treeModel->getIndicatorByName(vysota_zdaniya);
    Pakazatel * p_skopost_veter = m_treeModel->getIndicatorByName(max_wind_velocity);
    Pakazatel * p_coeff_rekuperator = m_treeModel->getIndicatorByName(coeff_recuperation);

    double lvent1 = 30 * p_kol_kvartiryi->calcValue() ;
    double lvent2 = 0.35 * 3 * p_area_living_space->calcValue();
    double lvent = 0;
    if (lvent1 > lvent2) lvent = lvent1;
    else lvent = lvent2;

    double y_ext = 12.68; //TODO Constant?
    double y_int = 12.68; //TODO Constant?
    double v = p_skopost_veter->calcValue();
    double delta_P = 10;
    double Gn_ok = 5.00 ; //From snip
    double Gn_dv = 7.00;
    double n_vent = 60; // TODO get from model or constant?
    double n_info = 168;
    double rho_vozdukh = 353/(273 + 0.0);
    double k_eff = p_coeff_rekuperator->calcValue();

    double delta_P_okna = paznostDavlenie(0.28, y_int, v, y_ext);
    double delta_P_dver = paznostDavlenie(0.55, y_int, v, y_ext);
    double R_ok = soprativlenieVozdukhProniknovenie(Gn_ok, delta_P_okna, delta_P);
    double R_dv = soprativlenieVozdukhProniknovenie(Gn_dv,delta_P_dver,delta_P);
    double A_ok = p_area_windows->calcValue();
    double A_dv = p_area_doors->calcValue();
    double G_inf = kolichestvoinfiltrvozdukh(delta_P_dver, A_ok, R_dv, A_dv, R_ok, delta_P_okna);
    double lpritok = 3 * p_area_living_space->calcValue();


    double B_v = p_volume_reduction->calcValue();
    double V_otop = p_heated_volume->calcValue();
    double n_v1 = lvent/ (B_v * V_otop );

    double n_v2 = (((lvent * n_vent)/168) + (G_inf * n_info)/(B_v * V_otop) );

    double n_v3 = ((G_inf * n_info)/(168 * rho_vozdukh)/(B_v * V_otop));

    double n = n_v1 + n_v2 + n_v3 ;

    return n;

}

double EnergyPassportModel::udelnayateplozashita()
{
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByName(volume_heated_space);
    double V_otop = p_heated_volume->calcValue();
    QList<Entity *> frags;
    QStringList filter;
    ctx->getFragments(frags,filter);
    double total = totalCalcTeploZashita(frags);

    double res = (1/V_otop) * total;

    return res;
}

double EnergyPassportModel::udelnayateplozashitaRaschet()
{
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByName(volume_heated_space);
    double V_otop = p_heated_volume->calcValue();
    double GSOP = raschetGSOP();

    double res ;

    if (V_otop > 960){
        res = (0.16 + (10/sqrt(V_otop)))/((0.00013*GSOP) + 0.61);
    }else{
        res = ((4.74)/(((0.00013*GSOP) + 0.61))) * (1 /(pow(V_otop,1/3.0)));
    }

    return res;
}

double EnergyPassportModel::udelnayaventilyatsii()
{
    Pakazatel * p_coeff_rekuperator = m_treeModel->getIndicatorByName(coeff_recuperation);
    Pakazatel * p_volume_reduction = m_treeModel->getIndicatorByName(coeff_volume_reduction);

    double k_eff = p_coeff_rekuperator->calcValue();
    double B_v = p_volume_reduction->calcValue();
    double n = kratnostvozdukhobmen();

    double K_vent = 0.28 * B_v * n * (1 - k_eff);
    return K_vent;
}

double EnergyPassportModel::udelnayatenlovyidelenie()
{
    if (m_treeModel == nullptr) return 0;

    Pakazatel * p_bytovoi = m_treeModel->getIndicatorByName(specific_Thermal_Emission);
    Pakazatel * p_area_living_space = m_treeModel->getIndicatorByName(area_living_space);
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByName(volume_heated_space);
    Pakazatel * p_temp_ext = m_treeModel->getIndicatorByName(temp_air_external);
    Pakazatel * p_temp_int = m_treeModel->getIndicatorByName(temp_air_internal);


    double q_byit = p_bytovoi->calcValue();
    double a_jiloi = p_area_living_space->calcValue();
    double V_otop = p_heated_volume->calcValue();
    double t_int = p_temp_int->calcValue();
    double t_ext = p_temp_ext->calcValue();

    double K_byit = (q_byit * a_jiloi)/(V_otop * (t_int - t_ext));

    return K_byit;
}

double EnergyPassportModel::raschetGSOP()
{
    Pakazatel * p_temp_int = m_treeModel->getIndicatorByName(temp_air_internal);
    Pakazatel * p_temp_avg_ext = m_treeModel->getIndicatorByName(heating_period_temp_avg);
    Pakazatel * p_heating_period = m_treeModel->getIndicatorByName(heating_period_days);

    double temp_int = p_temp_int->calcValue();
    double temp_ext_avg = p_temp_avg_ext->calcValue();
    double otop_period = p_heating_period->calcValue();
    double GSOP = (temp_int - temp_ext_avg)* otop_period;

    return GSOP;
}

double EnergyPassportModel::koeffOtlichieVnutrVneshTemp(const double tnorm)
{
    Pakazatel * p_temp_int = m_treeModel->getIndicatorByName(temp_air_internal);
    Pakazatel * p_temp_avg_ext = m_treeModel->getIndicatorByName(heating_period_temp_avg);
    Pakazatel * p_temp_ext = m_treeModel->getIndicatorByName(temp_air_external);

    double temp_int = p_temp_int->calcValue();
    double temp_ext_avg = p_temp_avg_ext->calcValue();
    double temp_ext = p_temp_ext->calcValue();

    double res = (tnorm - temp_ext_avg)/(temp_int - temp_ext);

    return res;
}

double EnergyPassportModel::subCalcTeploZashita(const double tnorm,const double area, const double rprev)
{
    double n_ij = koeffOtlichieVnutrVneshTemp(tnorm);
    double res = n_ij * (area/rprev);

    return res;
}

double EnergyPassportModel::totalCalcTeploZashita(QList<Entity *> fragments)
{
    double res = 0.0,sub_res;
    double tnorm,area,rprev;
    Fragment *f;
    for(int i=0;i < fragments.count() ; i++){
        f = (Fragment*)fragments[i];
        tnorm = f->tnorm();
        area = f->area();
        rprev = f->resistance();

        sub_res = subCalcTeploZashita(tnorm,area,rprev);
        res += sub_res;
    }

    return res;
}

double EnergyPassportModel::saveTreeModeltoDB()
{
    QList<TreeItem*> items;
    m_treeModel->getIndicators(items);

    for(int i = 0; i < items.count();i++){
        Pakazatel *p = m_treeModel->getIndicatorByName(items[i]->data(0).toString());
        ctx->insertPakazatel(p);
    }
}


double EnergyPassportModel::udelnayatenlopostunleniesontse()
{
    if (m_treeModel == nullptr) return 0;

    Pakazatel *p_area_ok1 = m_treeModel->getIndicatorByName(area_facing_N);
    Pakazatel *p_area_ok2 = m_treeModel->getIndicatorByName(area_facing_NE);
    Pakazatel *p_area_ok3 = m_treeModel->getIndicatorByName(area_facing_E);
    Pakazatel *p_area_ok4 = m_treeModel->getIndicatorByName(area_facing_SE);
    Pakazatel *p_area_ok5 = m_treeModel->getIndicatorByName(area_facing_S);
    Pakazatel *p_area_ok6 = m_treeModel->getIndicatorByName(area_facing_SW);
    Pakazatel *p_area_ok7 = m_treeModel->getIndicatorByName(area_facing_W);
    Pakazatel *p_area_ok8 = m_treeModel->getIndicatorByName(area_facing_NW);
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByName(volume_heated_space);


    double t_1ok = 0.8;
    double t_2ok = 0.74;
    double t_1fon = 0;
    double t_2fon = 0;
    double A_ok1 = p_area_ok1->calcValue();
    double A_ok2 = p_area_ok2->calcValue();
    double A_ok3 = p_area_ok3->calcValue();
    double A_ok4 = p_area_ok4->calcValue();
    double A_ok5 = p_area_ok5->calcValue();
    double A_ok6 = p_area_ok6->calcValue();
    double A_ok7 = p_area_ok7->calcValue();
    double A_ok8 = p_area_ok8->calcValue();
    double V_otop = p_heated_volume->calcValue();



    double I_1 = 612;
    double I_2 = 677;
    double I_3 = 911;
    double I_4 = 1285;
    double I_5 = 1465;
    double I_6 = 1285;
    double I_7 = 911;
    double I_8 = 677;

    double Q_rad = t_1ok * t_2ok * (A_ok1 * I_1 + A_ok2 * I_2 + A_ok3 * I_3 + A_ok4* I_4 +
                                  A_ok5 * I_5 + A_ok6 * I_6 + A_ok7 * I_7 + A_ok8* I_8) ;
    //+ t_1fon * t_2fon *A_fon*I_gor
    double GSOP = raschetGSOP();
    double K_rad = (11.6 * Q_rad)/(V_otop * GSOP);
    return K_rad;
}

TreeModel *EnergyPassportModel::treeModel()
{
    return m_treeModel;
}

QSqlRelationalTableModel *EnergyPassportModel::pakazatelModel()
{
    return ctx->getPakazatelModel();
}

QSqlRelationalTableModel *EnergyPassportModel::fragmentModel()
{
    return ctx->getFragmentModel();
}
















