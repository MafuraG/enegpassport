#include "energypassportmodel.h"
#include "excelreport.h"
#include "treeitem.h"
#include <QFile>
#include <QTextStream>
#include <QLocale>
#include <xlsxdocument.h>
#include <xlsxworkbook.h>


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
               << Pakazatel::D_FactValue
               << Pakazatel::D_Calculated
               << Pakazatel::D_Numeration
               << Pakazatel::ID;


    //QFile file(":/menu_structure.txt");
    //file.open(QIODevice::ReadOnly);
    QList<Entity *> plist;
    QStringList filter;
    ctx->getPakazateli(plist,filter);
    m_treeModel = new TreeModel(headers,plist);
    //file.close();

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

    Pakazatel *p_area = m_treeModel->getIndicatorByID(area_boundary);
    Pakazatel *p_heated_volume = m_treeModel->getIndicatorByID(volume_heated_space);

    if (p_area != nullptr && p_heated_volume != nullptr && p_heated_volume->calcValue() != 0){
        double area = p_area->calcValue(); delete p_area;
        double volume = p_heated_volume->calcValue(); delete p_heated_volume;
        return area / volume;
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
    //Gинф = (Aок / Rок) · (DPок / 10)2/3 + Aдв / Rдв) · (DPдв / 10)1/2
    double G_inf = ( A_ok / R_ok )* (pow((delta_P_okna/10.0),0.666666667)) + (A_dv/R_dv) * sqrt(delta_P_dver/10);
    return G_inf;
}

double EnergyPassportModel::lVentilyatsi(EnergyPassportModel::TipZdaniya z_type){
    Pakazatel * p_area_living_space = m_treeModel->getIndicatorByID(area_living_space);
    double Ap = p_area_living_space->calcValue();
    double lvent = 0;

    switch(z_type){
        case EnergyPassportModel::type1 : lvent = 4 * Ap;
        case EnergyPassportModel::type2 : lvent = 5 * Ap;
        case EnergyPassportModel::type3 : lvent = 7 * Ap;
        case EnergyPassportModel::type4 : lvent = 10 * Ap;
    }

    return lvent;
}

double EnergyPassportModel::kratnostvozdukhobmen()
{
    if (m_treeModel == nullptr) return 0;

    Pakazatel * p_kol_kvartiryi = m_treeModel->getIndicatorByID(kol_kvartir);
    Pakazatel * p_area_living_space = m_treeModel->getIndicatorByID(area_living_space);
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByID(volume_heated_space);
    Pakazatel * p_volume_reduction = m_treeModel->getIndicatorByID(coeff_volume_reduction);
    Pakazatel * p_area_doors = m_treeModel->getIndicatorByID(area_doors);
    Pakazatel * p_area_windows = m_treeModel->getIndicatorByID(area_windows_balcony);
    //Pakazatel * p_vysota_zdanya = m_treeModel->getIndicatorByID(vysota_zdaniya);
    Pakazatel * p_skopost_veter = m_treeModel->getIndicatorByID(max_wind_velocity);
    Pakazatel * p_coeff_rekuperator = m_treeModel->getIndicatorByID(coeff_recuperation);
    Pakazatel * p_temp_avg_ext = m_treeModel->getIndicatorByID(heating_period_temp_avg);
    Pakazatel * p_Gn_ok = m_treeModel->getIndicatorByID(norm_vozdukh_pronisaemost_okon);
    Pakazatel * p_Gn_dv = m_treeModel->getIndicatorByID(norm_vozdukh_pronisaemost_dver);


    double lvent = lVentilyatsi(m_tzdaniya);

    double y_ext = 12.68; //TODO Constant?
    double y_int = 12.68; //TODO Constant?
    double v = p_skopost_veter->calcValue();
    double delta_P = 10;
    double Gn_ok = p_Gn_ok->calcValue() ; //From snip
    double Gn_dv = p_Gn_dv->calcValue() ;
    double n_vent = 60; // TODO get from model or constant?
    double n_info = 168;
    double temp_ext_avg = p_temp_avg_ext->calcValue();
    double rho_vozdukh = 353/(273 + temp_ext_avg);
    //double k_eff = p_coeff_rekuperator->calcValue();

    double delta_P_okna = paznostDavlenie(0.28, y_int, v, y_ext);
    double delta_P_dver = paznostDavlenie(0.55, y_int, v, y_ext);
    double R_ok = soprativlenieVozdukhProniknovenie(Gn_ok, delta_P_okna, delta_P);
    double R_dv = soprativlenieVozdukhProniknovenie(Gn_dv,delta_P_dver,delta_P);
    double A_ok = p_area_windows->calcValue();
    double A_dv = p_area_doors->calcValue();
    double G_inf = kolichestvoinfiltrvozdukh(delta_P_dver, A_ok, R_dv, A_dv, R_ok, delta_P_okna);
    //double lpritok = 3 * p_area_living_space->calcValue();


    double B_v = p_volume_reduction->calcValue();
    double V_otop = p_heated_volume->calcValue();
    double n_v1 = lvent/ (B_v * V_otop );

    double n_v2 = (((lvent * n_vent)/168) + (G_inf *0.8* n_info)/(168 * rho_vozdukh))/(B_v * V_otop) ;
    double n_v3 = ((G_inf * n_info)/(168 * rho_vozdukh))/(B_v * V_otop);

    double n = n_v1 + n_v2 + n_v3 ;

    delete p_kol_kvartiryi ;
    delete p_area_living_space ;
    delete p_heated_volume ;
    delete p_volume_reduction ;
    delete p_area_doors ;
    delete p_area_windows;
    //Pakazatel * p_vysota_zdanya = m_treeModel->getIndicatorByID(vysota_zdaniya);
    delete p_skopost_veter ;
    delete p_coeff_rekuperator ;
    delete p_temp_avg_ext ;

    return n;

}

double EnergyPassportModel::udelnayateplozashita()
{
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByID(volume_heated_space);
    double V_otop = p_heated_volume->calcValue();
    QList<Entity *> frags;
    QStringList filter;
    ctx->getFragments(frags,filter);
    double total = totalCalcTeploZashita(frags);

    double res = (1/V_otop) * total;
    delete p_heated_volume;

    return res;
}

double EnergyPassportModel::udelnayateplozashitaRaschet()
{
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByID(volume_heated_space);
    double V_otop = p_heated_volume->calcValue();
    double GSOP = raschetGSOP();

    double res ;

    if (V_otop > 960){
        res = (0.16 + (10/sqrt(V_otop)))/((0.00013*GSOP) + 0.61);
    }else{
        res = ((4.74)/(((0.00013*GSOP) + 0.61))) * (1 /(pow(V_otop,1/3.0)));
    }

    delete p_heated_volume;
    return res;
}

double EnergyPassportModel::udelnayaventilyatsii()
{
    Pakazatel * p_coeff_rekuperator = m_treeModel->getIndicatorByID(coeff_recuperation);
    Pakazatel * p_volume_reduction = m_treeModel->getIndicatorByID(coeff_volume_reduction);

    double k_eff = p_coeff_rekuperator->calcValue();
    double B_v = p_volume_reduction->calcValue();
    double n = kratnostvozdukhobmen();

    double K_vent = 0.28 * B_v * n * (1 - k_eff);

    delete p_coeff_rekuperator;
    delete p_volume_reduction;
    return K_vent;
}

double EnergyPassportModel::udelnayabwitavayatenlovyidelenie()
{
    if (m_treeModel == nullptr) return 0;

    Pakazatel * p_bytovoi = m_treeModel->getIndicatorByID(thermal_E_building);
    Pakazatel * p_area_living_space = m_treeModel->getIndicatorByID(area_living_space);
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByID(volume_heated_space);
    //Pakazatel * p_temp_ext = m_treeModel->getIndicatorByID(temp_air_external);
    Pakazatel * p_temp_int = m_treeModel->getIndicatorByID(temp_air_internal);
    Pakazatel * p_temp_avg_ext = m_treeModel->getIndicatorByID(heating_period_temp_avg);


    double q_byit = p_bytovoi->calcValue();
    double a_jiloi = p_area_living_space->calcValue();
    double V_otop = p_heated_volume->calcValue();
    double t_int = p_temp_int->calcValue();
    double t_ext = p_temp_avg_ext->calcValue();

    double K_byit = (q_byit * a_jiloi)/(V_otop * (t_int - t_ext));

    delete p_bytovoi;
    delete p_area_living_space;
    delete p_heated_volume;
    delete p_temp_int;
    delete p_temp_avg_ext;

    return K_byit;
}

inline double EnergyPassportModel::round(double n , unsigned d){
    return floor( n * pow(10.  ,d) + 0.5) / pow(10.,d);
}
EnergyPassportModel::TipZdaniya EnergyPassportModel::tzdaniya() const
{
    return m_tzdaniya;
}

void EnergyPassportModel::setTzdaniya(const EnergyPassportModel::TipZdaniya &tzdaniya)
{
    m_tzdaniya = tzdaniya;
}


double EnergyPassportModel::raschetGSOP()
{
    Pakazatel * p_temp_int = m_treeModel->getIndicatorByID(temp_air_internal);
    Pakazatel * p_temp_avg_ext = m_treeModel->getIndicatorByID(heating_period_temp_avg);
    Pakazatel * p_heating_period = m_treeModel->getIndicatorByID(heating_period_days);

    double temp_int = p_temp_int->calcValue();
    double temp_ext_avg = p_temp_avg_ext->calcValue();
    double otop_period = p_heating_period->calcValue();
    double GSOP = (temp_int - temp_ext_avg)* otop_period;

    delete p_temp_avg_ext;
    delete p_temp_int;
    delete p_heating_period;

    return round(GSOP,0);
}

double EnergyPassportModel::koeffOtlichieVnutrVneshTemp(const double tnorm)
{
    Pakazatel * p_temp_int = m_treeModel->getIndicatorByID(temp_air_internal);
    Pakazatel * p_temp_avg_ext = m_treeModel->getIndicatorByID(heating_period_temp_avg);
    //Pakazatel * p_temp_ext = m_treeModel->getIndicatorByID(temp_air_external);

    double temp_int = p_temp_int->calcValue();
    double temp_ext_avg = p_temp_avg_ext->calcValue();
    //double temp_ext = p_temp_ext->calcValue();

    double res = (tnorm - temp_ext_avg)/(temp_int - temp_ext_avg);
    delete p_temp_avg_ext;
    delete p_temp_int;
    //delete p_temp_ext;

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

void EnergyPassportModel::saveTreeModeltoDB()
{
    QList<TreeItem*> items;    
    m_treeModel->getIndicators(items);

    ctx->startTransaction();
    for(int i = 0; i < items.count();i++){
        Pakazatel *c = m_treeModel->getIndicatorByID(items[i]->data(7).toInt());
        if (c == nullptr) continue;
        ctx->insertPakazatel(c);
        delete c;
    }
    ctx->endTransaction();
}

void EnergyPassportModel::saveModelDatatoFile(const QString fname)
{
    QList<TreeItem*> list;
    m_treeModel->getIndicators(list);
    QLocale russian(QLocale::Russian);

    QFile file(fname);
    if (file.open(QIODevice::ReadWrite)){
        QTextStream out(&file);
        out<<Pakazatel::D_Name <<";"<<Pakazatel::D_CalcValue<<";"<<Pakazatel::D_ParentID<<"\n";
        for (int i = 0 ; i < list.count(); i++){
            TreeItem *t = list[i];
            //t not null(exists) and is a leaf on tree (no child elements)
            if (t != nullptr){
                //find out if leaf is calculated
                QVariant val = t->data(7);
                QString p = "--";
                QVariant cal = t->data(3) ;
                if (t->parent() != nullptr) p = t->parent()->data(0).toString();
                out << val.toInt() <<";"<<russian.toString(cal.toDouble())<<"\n";

            }
        }
    }
}

void EnergyPassportModel::loadModelDataFromFile(const QString fname)
{
    QList<TreeItem*> list;
    m_treeModel->getIndicators(list);
    //RegEx to check if string is composed of digits
    QLocale russian(QLocale::Russian);

    QFile file(fname);
    if (file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        QString line;
        while (!in.atEnd()){
            line = in.readLine();
            QStringList parts = line.split(QString(";"));
            if (parts.count() > 1){
                Pakazatel p;
                QString calcval = parts[1];                
                QString id = parts[0];
                bool isnum;
                double dcalcval = russian.toDouble(calcval,&isnum);
                if (isnum){
                    p.setCalcValue(dcalcval);
                    //p.setCalculated(false);
                    p.setId(id.toInt());
                    m_treeModel->setIndicatorByID(&p);
                }
                else continue;                
            }
        }
    }
}


double EnergyPassportModel::udelnayatenlopostunleniesontse()
{
    if (m_treeModel == nullptr) return 0;

    Pakazatel *p_area_ok1 = m_treeModel->getIndicatorByID(area_facing_N);
    Pakazatel *p_area_ok2 = m_treeModel->getIndicatorByID(area_facing_NE);
    Pakazatel *p_area_ok3 = m_treeModel->getIndicatorByID(area_facing_E);
    Pakazatel *p_area_ok4 = m_treeModel->getIndicatorByID(area_facing_SE);
    Pakazatel *p_area_ok5 = m_treeModel->getIndicatorByID(area_facing_S);
    Pakazatel *p_area_ok6 = m_treeModel->getIndicatorByID(area_facing_SW);
    Pakazatel *p_area_ok7 = m_treeModel->getIndicatorByID(area_facing_W);
    Pakazatel *p_area_ok8 = m_treeModel->getIndicatorByID(area_facing_NW);
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByID(volume_heated_space);
    Pakazatel *p_I1 = m_treeModel->getIndicatorByID(sontse_vert_rad_avg_N);
    Pakazatel *p_I2 = m_treeModel->getIndicatorByID(sontse_vert_rad_avg_NE);
    Pakazatel *p_I3 = m_treeModel->getIndicatorByID(sontse_vert_rad_avg_E);
    Pakazatel *p_I4 = m_treeModel->getIndicatorByID(sontse_vert_rad_avg_SE);
    Pakazatel *p_I5 = m_treeModel->getIndicatorByID(sontse_vert_rad_avg_S);
    Pakazatel *p_I6 = m_treeModel->getIndicatorByID(sontse_vert_rad_avg_SW);
    Pakazatel *p_I7 = m_treeModel->getIndicatorByID(sontse_vert_rad_avg_W);
    Pakazatel *p_I8 = m_treeModel->getIndicatorByID(sontse_vert_rad_avg_NW);
    Pakazatel *p_t_1ok = m_treeModel->getIndicatorByID(coeff_proniknovenie_sontse_okno);
    Pakazatel *p_t_2ok = m_treeModel->getIndicatorByID(coeff_zatenenie_okno);

    double t_1ok = p_t_1ok->calcValue();
    double t_2ok = p_t_2ok->calcValue();
    //double t_1fon = 0;
    //double t_2fon = 0;
    double A_ok1 = p_area_ok1->calcValue();
    double A_ok2 = p_area_ok2->calcValue();
    double A_ok3 = p_area_ok3->calcValue();
    double A_ok4 = p_area_ok4->calcValue();
    double A_ok5 = p_area_ok5->calcValue();
    double A_ok6 = p_area_ok6->calcValue();
    double A_ok7 = p_area_ok7->calcValue();
    double A_ok8 = p_area_ok8->calcValue();
    double V_otop = p_heated_volume->calcValue();



    double I_1 = p_I1->calcValue();
    double I_2 = p_I2->calcValue();
    double I_3 = p_I3->calcValue();
    double I_4 = p_I4->calcValue();
    double I_5 = p_I5->calcValue();
    double I_6 = p_I6->calcValue();
    double I_7 = p_I7->calcValue();
    double I_8 = p_I8->calcValue();

    double Q_rad = t_1ok * t_2ok * (A_ok1 * I_1 + A_ok2 * I_2 + A_ok3 * I_3 + A_ok4* I_4 +
                                  A_ok5 * I_5 + A_ok6 * I_6 + A_ok7 * I_7 + A_ok8* I_8) ;
    //+ t_1fon * t_2fon *A_fon*I_gor
    double GSOP = raschetGSOP();
    double K_rad = (11.6 * Q_rad)/(V_otop * GSOP);

    delete p_area_ok1 ;
    delete p_area_ok2 ;
    delete p_area_ok3 ;
    delete p_area_ok4 ;
    delete p_area_ok5 ;
    delete p_area_ok6 ;
    delete p_area_ok7 ;
    delete p_area_ok8 ;
    delete  p_heated_volume ;
    delete p_I1 ;
    delete p_I2 ;
    delete p_I3 ;
    delete p_I4 ;
    delete p_I5 ;
    delete p_I6 ;
    delete p_I7 ;
    delete p_I8 ;
    delete p_t_1ok ;
    delete p_t_2ok ;

    return K_rad;
}

double EnergyPassportModel::koeffsnijenieteplopastuplenia()
{
    //ОКРУГЛ(0,7+0,000025*('Эн паспорт 1'!E31-1000);2)
    double GSOP = raschetGSOP();
    double res = (0.7 + (0.000025 * (GSOP - 1000)));
    return res;
}

double EnergyPassportModel::udelniraskhodteplovoienergii()
{
    Pakazatel *p_thermal_loss_reduction = m_treeModel->getIndicatorByID(coeff_reduction);
    Pakazatel *p_extra_loss = m_treeModel->getIndicatorByID(coeff_additional);
    Pakazatel *p_regulation = m_treeModel->getIndicatorByID(coeff_reduction);

    double K_ob = udelnayateplozashita();
    double K_vent = udelnayaventilyatsii();
    double K_bwit = udelnayabwitavayatenlovyidelenie();
    double K_rad = udelnayatenlopostunleniesontse();
    double n = koeffsnijenieteplopastuplenia();
    double Z = p_regulation->calcValue();
    double X = p_thermal_loss_reduction->calcValue();
    double bh = p_extra_loss->calcValue();

    //qрот=[Kоб + Квент - (Кбыт + Крад) * n * z] * (1 - x) * bh
    double q_rot = (K_ob + K_vent - ((K_bwit + K_rad) * n * Z)) * ((1 - X) * bh);

    return q_rot;

}

double EnergyPassportModel::udelniiraskhodnaotopperiod()
{
    //q=Qгодот/Аот

    Pakazatel * p_area_all_floors = m_treeModel->getIndicatorByID(area_all_floors);

    double A_ot =  p_area_all_floors->calcValue();
    double q_godrot = raskhodnaotopperiod();
    double q = q_godrot / A_ot ;

    return q ;
}

double EnergyPassportModel::raskhodnaotopperiod()
{
    //Qгодот = 0,024*ГСОП*Vот*qрот
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByID(volume_heated_space);

    double GSOP = raschetGSOP();
    double V_otop = p_heated_volume->calcValue();
    double q_rot = udelniraskhodteplovoienergii();

    double Q_godot = (0.024 * GSOP * V_otop * q_rot)/10 ;

    return Q_godot;

}

double EnergyPassportModel::obshieteplopoteriizaperiod()
{
    //Qгодобщ = 0,024*ГСОП*Vот*(Коб+Квент)
    Pakazatel * p_heated_volume = m_treeModel->getIndicatorByID(volume_heated_space);

    double GSOP = raschetGSOP();
    double V_otop = p_heated_volume->calcValue();
    double K_ob = udelnayateplozashita();
    double K_vent = udelnayaventilyatsii();

    double Q_god_obsh = (0.024 * GSOP * V_otop * (K_ob + K_vent))/10;

    return Q_god_obsh;

}

void EnergyPassportModel::raschetPakazateli()
{
    //TO coeff osteklenie not known
    //Add it after asking
    Pakazatel p;
    p.setCalculated(true);

    //GSOP
    p.setId(heating_period_degree_days);
    p.setCalcValue(raschetGSOP());
    m_treeModel->setIndicatorByID(&p);


    //koeff kompaknost
    p.setId(coeff_compactness);
    p.setCalcValue(kompaktnost());
    m_treeModel->setIndicatorByID(&p);

    //kratnoist vozdukhoobmen
    p.setId(ventilation_cycle);
    p.setCalcValue(kratnostvozdukhobmen());
    m_treeModel->setIndicatorByID(&p);

    //udelnaya teplozashita
    p.setId(specific_Thermal_Protection);
    p.setCalcValue(udelnayateplozashita());
    m_treeModel->setIndicatorByID(&p);

    //udelnaya ventilyatsionaya charactistics
    p.setId(specific_Ventilation_Characteristics);
    p.setCalcValue(udelnayaventilyatsii());
    m_treeModel->setIndicatorByID(&p);

    //udelnaya bwitovaya teplovidelenie
    p.setId(specific_Thermal_Emission);
    p.setCalcValue(udelnayabwitavayatenlovyidelenie());
    m_treeModel->setIndicatorByID(&p);

    //udelnaya teplopactuplenie
    p.setId(specific_Solar_Reception);
    p.setCalcValue(udelnayatenlopostunleniesontse());
    m_treeModel->setIndicatorByID(&p);

    //coeff snijenie isplolzovanie teplopotreblenie
    p.setId(coeff_reduction);
    p.setCalcValue(koeffsnijenieteplopastuplenia());
    m_treeModel->setIndicatorByID(&p);

    //raschet udelnii raskhod thermal energy
    p.setId(thermal_usage_calc);
    p.setCalcValue(udelniraskhodteplovoienergii());
    m_treeModel->setIndicatorByID(&p);

    //udelnii raskhod thermal energy for period
    p.setId(thermal_usage_spec_heating_season);
    p.setCalcValue(udelniiraskhodnaotopperiod());
    m_treeModel->setIndicatorByID(&p);

    //raskhod teplovoi energii za otopitel'nii period
    p.setId(thermal_usage_calc_heating_season);
    p.setCalcValue(raskhodnaotopperiod());
    m_treeModel->setIndicatorByID(&p);

    //obshie teplopaterya zdaniya
    p.setId(thermal_wastage_heating_season);
    p.setCalcValue(obshieteplopoteriizaperiod());
    m_treeModel->setIndicatorByID(&p);

}

void EnergyPassportModel::writeXlsReport(const QString template_, const QString output_){

    QHash<QString,QString> addr;
    QXlsx::Document doc_t(template_);
    QXlsx::Workbook *wb = doc_t.workbook();
    QXlsx::Worksheet *ws =(QXlsx::Worksheet *) wb->sheet(0);

    ExcelReport util;

    util.linkAddrrToVal(ws,addr);

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
















